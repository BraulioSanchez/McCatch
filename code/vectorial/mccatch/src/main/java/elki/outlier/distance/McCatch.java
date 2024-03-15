package elki.outlier.distance;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.Locale;
import java.util.Objects;

import org.apache.commons.lang3.ArrayUtils;
import org.apache.commons.math3.util.FastMath;

import elki.Algorithm;
import elki.data.DoubleVector;
import elki.data.ModifiableHyperBoundingBox;
import elki.data.NumberVector;
import elki.data.type.TypeInformation;
import elki.data.type.TypeUtil;
import elki.database.datastore.DataStoreFactory;
import elki.database.datastore.DataStoreUtil;
import elki.database.datastore.WritableDoubleDataStore;
import elki.database.ids.ArrayModifiableDBIDs;
import elki.database.ids.DBIDIter;
import elki.database.ids.DBIDRef;
import elki.database.ids.DBIDUtil;
import elki.database.ids.HashSetModifiableDBIDs;
import elki.database.ids.ModifiableDBIDs;
import elki.database.relation.DoubleRelation;
import elki.database.relation.MaterializedDoubleRelation;
import elki.database.relation.Relation;
import elki.database.relation.RelationUtil;
import elki.distance.Distance;
import elki.distance.SpatialPrimitiveDistance;
import elki.distance.minkowski.EuclideanDistance;
import elki.index.tree.spatial.SpatialEntry;
import elki.index.tree.spatial.SpatialPointLeafEntry;
import elki.logging.Logging;
import elki.outlier.OutlierAlgorithm;
import elki.result.outlier.OutlierResult;
import elki.result.outlier.OutlierScoreMeta;
import elki.result.outlier.ProbabilisticOutlierScore;
import elki.utilities.exceptions.AbortException;
import elki.utilities.optionhandling.OptionID;
import elki.utilities.optionhandling.Parameterizer;
import elki.utilities.optionhandling.constraints.CommonConstraints;
import elki.utilities.optionhandling.parameterization.Parameterization;
import elki.utilities.optionhandling.parameters.DoubleParameter;
import elki.utilities.optionhandling.parameters.IntParameter;
import elki.utilities.optionhandling.parameters.ObjectParameter;

/**
 * McCatch for dimensional datasets.
 */
public class McCatch<O extends NumberVector> implements OutlierAlgorithm {

	/**
	 * Class logger.
	 */
	private static final Logging LOG = Logging.getLogger(McCatch.class);

	private int a;

	private double b;

	private double c;

	private Distance<? super O> distance;

	private int output;

	/**
	 * Internal members
	 */
	final protected int LEAF_MINIMUM = 50;
	final int Y = 0, PLATEAU_BEGIN = 1, X = 2, NEAREST_INLIER = 3;
	private int pageSize;

	/**
	 * Constructor.
	 */
	public McCatch(int a, double b, double c, Distance<? super O> distance, int output) {
		super();
		this.a = a;
		this.b = b;
		this.c = c;
		this.distance = distance;
		this.output = output;
	}

	@Override
	public TypeInformation[] getInputTypeRestriction() {
		return TypeUtil.array(distance.getInputTypeRestriction());
	}

	/**
	 * Run the algorithm.
	 * 
	 * @param P Relation
	 * @return Result
	 */
	public OutlierResult run(Relation<O> P) {
		Locale.setDefault(Locale.ENGLISH);

		// BEGIN: Define the neighborhood radii
		System.out.println("Define the neighborhood radii ...");
		final RStarTreeIndex2<O> T = buildTree(P);
		final double[] R = calculateRadii(T);
		// END: Define the neighborhood radii

		// BEGIN: Build the 'Oracle' plot
		System.out.println("Build the 'Oracle' plot ...");
		OPlot O = buildOPlot(P, T, R);
		// END: Build the 'Oracle' plot

		// BEGIN: Spot the microclusters
		System.out.println("Spot the microclusters ...");
		MicroCluster[] M = spotMCs(P, O, R);
		// END: Spot the microclusters

		// BEGIN: Compute the anomaly scores
		System.out.println("Compute the anomaly scores ...");
		WritableDoubleDataStore W = scoreMCs(P, M, O, R);
		// END: Compute the anomaly scores

		if (this.output == 0 || this.output == 1) { // output == 0, full output
			for (int j = 0; j < M.length; j++) {
				for (int i = 0; i < M[j].size(); i++) {
					@SuppressWarnings("deprecation")
					int id = DBIDUtil.asInteger(M[j].members.get(i)) - 1;
					System.out.println(String.format("mj%d,%d,%.12f", j, id, M[j].s));
				}
			}
			if (this.output == 1) { // only microclusters output
				System.exit(0);
			}
		}

		// output == 2, only score per point 
		DoubleRelation scores = new MaterializedDoubleRelation("McCatch score per point", P.getDBIDs(), W);
		OutlierScoreMeta meta = new ProbabilisticOutlierScore();
		return new OutlierResult(meta, scores);
	}

	protected RStarTreeIndex2<O> buildTree(Relation<O> P) {
		try {
			final double relativeMinFill = 0.4;
			final int dim = RelationUtil.dimensionality(P);

			int cap = 0;
			ByteArrayOutputStream baos = new ByteArrayOutputStream();
			ObjectOutputStream oos = new ObjectOutputStream(baos);
			SpatialPointLeafEntry sl = new SpatialPointLeafEntry(DBIDUtil.importInteger(0), new double[dim]);
			while (cap <= (int) (LEAF_MINIMUM / relativeMinFill)) {
				sl.writeExternal(oos);
				oos.flush();
				cap++;
			}
			// the last one caused the page to overflow.
			pageSize = baos.size() - 1;
		} catch (IOException e) {
			throw new AbortException("Error determining page sizes.", e);
		}

		RStarTreeIndex2<O> tree = new RStarTreeIndex2<>(P, pageSize);
		tree.initialize();
		tree.insertAll(P.getDBIDs());

		return tree;
	}

	protected double[] calculateRadii(RStarTreeIndex2<O> T) {
		SpatialEntry entry = T.getRootEntry();
		SpatialPrimitiveDistance<? super O> distanceFunction = (SpatialPrimitiveDistance<? super O>) distance;

		ModifiableHyperBoundingBox mbr = new ModifiableHyperBoundingBox(entry);
		@SuppressWarnings("unchecked")
		O vMax = (O) new DoubleVector(mbr.getMaxRef());
		@SuppressWarnings("unchecked")
		O vMin = (O) new DoubleVector(mbr.getMinRef());
		final double l = distanceFunction.distance(vMin, vMax);
		final double[] R = new double[a + 1];
		for (int i = 0; i < R.length; i++) {
			R[i] = l / FastMath.pow(2, i);
		}
		return R;
	}

	protected OPlot buildOPlot(Relation<O> P, RStarTreeIndex2<O> T, double[] R) {
		final int n = P.size();
		c = Math.ceil(n * c);

		final OPlot O = new OPlot(n);

		// BEGIN: Count the neighbors
		final int[][] q = new int[n][a + 1];
		final int[] qe = new int[n];
		final boolean[] enoughNeighbors = new boolean[n];
		Arrays.fill(enoughNeighbors, false);

		ArrayModifiableDBIDs ids = DBIDUtil.newArray(P.getDBIDs());

		NCSJ<O> ncsj = new NCSJ<>();

		RStarTreeIndex2<O> _T = new RStarTreeIndex2<O>(P, pageSize);
		_T.initialize();
		_T.insertAll(P.getDBIDs());

		// from small to large radius
		for (int e = a; e >= 0; e--) { // begin FOR
			Arrays.fill(qe, 0);
			if (!ids.isEmpty()) {
				if (ids.size() != n) {
					ncsj.simJoin(P, _T, T, distance, qe, R[e]); // simJoin
				} else {
					ncsj.simJoin(P, _T, distance, qe, R[e]); // selfJoin
				}
			}

			if (!ids.isEmpty()) {
				ids = DBIDUtil.newArray();
				for (DBIDIter pi = P.getDBIDs().iter(); pi.valid(); pi.advance()) { // being FOR
					int i = (DBIDUtil.asInteger(pi) - 1);
					if (qe[i] > c) {
						enoughNeighbors[i] = true;
					}
					if (!enoughNeighbors[i]) {
						ids.add(pi);
					}
					q[i][e] = qe[i]; // stores the neighbors count
				} // end FOR
				if (ids.size() != n) {
					_T = new RStarTreeIndex2<O>(P, pageSize);
					_T.initialize();
					_T.insertAll(ids);
				}
			}
		} // end FOR
		for (int i = 0; i < n; i++) {
			q[i][0] = n; // completes the neighbors count
		}
		// END: Count the neighbors

		// BEGIN: Find the plateaus
		double[][] slopes = new double[n][a];
		for (int i = 0; i < n; i++) {
			Arrays.fill(slopes[i], Double.NaN);
		}

		ArrayUtils.reverse(R); // reverse radii

		for (DBIDIter pi = P.iterDBIDs(); pi.valid(); pi.advance()) {
			int i = (DBIDUtil.asInteger(pi) - 1);

			ArrayUtils.reverse(q[i]); // reverse qi

			// computes slopes by each point
			slopes[i][0] = (FastMath.log(q[i][1]) - FastMath.log(q[i][0])) //
					/ (FastMath.log(R[1]) - FastMath.log(R[0]));
			int endFirstPlateau = 0;
			int excused = a;
			for (int e = 1; e < a; e++) { // begin FOR
				if (q[i][e] > (int) c) { // avoid excused zone
					excused = e - 1;
					break;
				}
				if (q[i][e] < 2) { // end of first plateau
					endFirstPlateau = e;
				}

				slopes[i][e] = (FastMath.log(q[i][e + 1]) - FastMath.log(q[i][e])) //
						/ (FastMath.log(R[e + 1]) - FastMath.log(R[e]));
			} // end FOR

			double xi = R[endFirstPlateau];
			double yi = 0.;

			// look for middle plateau
			for (int e = endFirstPlateau; e < excused;) { // begin FOR
				if (slopes[i][e] <= b && q[i][e + 1] < (int) c) { // begin IF
					int _e = e + 1;
					for (; _e < a;) { // begin FOR
						if (slopes[i][_e] <= b && q[i][_e + 1] < (int) c) {
							_e++;
						} else {
							break;
						}
					} // end FOR

					if (_e < a) { // have middle plateau
						yi = R[_e] - R[e];
					} else {
						LOG.warning(String.format("%d does not have middle plateau!", i + 1));
					}

					e = _e;
				} // end IF
				else {
					e++;
				}
			} // end FOR

			// BEGIN: Build the 'Oracle' plot
			O.X[i] = xi;
			O.Y[i] = yi;
			// END: Build the 'Oracle' plot
		}
		// END: Find the plateaus

		return O;
	}

	protected MicroCluster[] spotMCs(Relation<O> P, OPlot O, double[] R) {
		// BEGIN: Compute the Cutoff d
		// Build the Histogram of 1NN Distances H
		int[] H = new int[a + 1];
		Arrays.fill(H, 0);
		for (DBIDIter pi = P.iterDBIDs(); pi.valid(); pi.advance()) {
			int i = (DBIDUtil.asInteger(pi) - 1);
			int e = ArrayUtils.indexOf(R, O.X[i]);
			H[e]++;
		}

		// Data-driven computation of d
		int _e = 0;
		for (int j = 0; j < H.length; j++) {
			if (H[_e] < H[j]) {
				_e = j;
			}
		}
		int __e = MDL(Arrays.copyOfRange(H, _e, H.length));
		final double d = R[__e + _e];
		// END: Compute the Cutoff d

		// BEGIN: Gel the outliers into microclusters
		ArrayModifiableDBIDs A = DBIDUtil.newArray();
		// Gel nonsingleton microclusters
		ArrayModifiableDBIDs M_ = DBIDUtil.newArray();
		double upx = -Double.MAX_VALUE;
		int e = 0;
		for (DBIDIter pi = P.iterDBIDs(); pi.valid(); pi.advance()) {
			int i = (DBIDUtil.asInteger(pi) - 1);
			if (O.X[i] >= d || O.Y[i] >= d) { // All outliers
				A.add(pi);
			}
			if (O.Y[i] >= d) { // Large Group 1NN D.
				M_.add(pi);
				if (upx < O.X[i]) { // Largest 1NN Dist.
					upx = O.X[i];
				}
			}
		}
		e = ArrayUtils.indexOf(R, upx);

		LinkedList<MicroCluster> M = new LinkedList<>();
		if (!M_.isEmpty()) {
			RStarTreeIndex2<O> T = new RStarTreeIndex2<O>(P, pageSize);
			T.initialize();
			T.insertAll(M_);

			final LinkedList<HashSetModifiableDBIDs> E = new LinkedList<>();
			CSJ<O> csj = new CSJ<>(true);
			csj.simJoin(T, distance, R[e + 1], E); // Find neighbors
			for (int i = 0; i < E.size() - 1; i++) { // Build neighborhood graph
				HashSetModifiableDBIDs Gi = E.get(i);
				for (int j = i + 1; j < E.size(); j++) {
					HashSetModifiableDBIDs Gj = E.get(j);
					for (DBIDIter pi = Gj.iter(); pi.valid(); pi.advance()) {
						if (Gi.contains(pi)) { // Find components
							Gi.addDBIDs(Gj);
							E.remove(j);
							j--;
							break;
						}
					}
				}
			}
			for (HashSetModifiableDBIDs Gj : E) {
				MicroCluster Mj = new MicroCluster();
				for (DBIDIter pi = Gj.iter(); pi.valid(); pi.advance()) {
					Mj.addMember(pi);
				}
				M.add(Mj); // Nonsingleton mcs
			}
		}

		// Gel singleton microclusters
		for (DBIDIter pi = DBIDUtil.difference(A, M_).iter(); pi.valid(); pi.advance()) {
			MicroCluster Mj = new MicroCluster();
			Mj.addMember(pi);
			M.add(Mj); // Add singleton mc
		}
		// END: Gel the outliers into microclusters

		MicroCluster[] M__ = new MicroCluster[M.size()];
		return M.toArray(M__);
	}

	protected WritableDoubleDataStore scoreMCs(Relation<O> P, MicroCluster[] M, OPlot O, double[] R) {
		final int n = P.size();
		final int t = RelationUtil.dimensionality(P);

		// BEGIN: Compute the distances to the nearest inliers
		// For outliers
		ArrayModifiableDBIDs A = DBIDUtil.newArray(); // All outliers
		for (MicroCluster Mj : M) {
			A.addDBIDs(Mj.members);
		}
		ModifiableDBIDs Inliers = DBIDUtil.difference(P.getDBIDs(), A); // All inliers

		final double[] g = new double[n];
		Arrays.fill(g, 0.);
		if (!A.isEmpty()) {
			RStarTreeIndex2<O> T = new RStarTreeIndex2<O>(P, pageSize); // Tree for outliers
			T.initialize();
			T.insertAll(A);

			RStarTreeIndex2<O> _T = new RStarTreeIndex2<O>(P, pageSize); // Tree for inliers
			_T.initialize();
			_T.insertAll(Inliers);

			NCSJ<O> ncsj = new NCSJ<>(false);
			int expectedSizeOfOutliers = 0;
			final boolean[] enoughNeighbors = new boolean[n];
			Arrays.fill(enoughNeighbors, false);
			final int[] f = new int[n];
			for (int e = 0; e <= a; e++) { // from small to large radii
				Arrays.fill(f, 0);
				ncsj.simJoin(P, T, _T, distance, f, R[e]); // simJoin

				for (DBIDIter pi = A.iter(); pi.valid(); pi.advance()) {
					int i = (DBIDUtil.asInteger(pi) - 1);
					if (f[i] > 0 && !enoughNeighbors[i]) {
						expectedSizeOfOutliers++;
						enoughNeighbors[i] = true;
						g[i] = R[e - 1 >= 0 ? e - 1 : 0]; // gi = distance to the nearest inlier
//						T.delete(pi);
					}
				}

				if (expectedSizeOfOutliers == A.size())
					break;
			}
		}

		// For inliers
		for (DBIDIter pi = Inliers.iter(); pi.valid(); pi.advance()) {
			int i = (DBIDUtil.asInteger(pi) - 1);
			g[i] = O.X[i]; // gi is the 1NN Distance of pi
		}
		// END: Compute the distances to the nearest inliers

		// BEGIN: Compute the scores per microcluster
		for (MicroCluster Mj : M) {
			double downgj = Double.MAX_VALUE;
			double averagexj = 0.;
			for (DBIDIter pi = Mj.members.iter(); pi.valid(); pi.advance()) {
				int i = (DBIDUtil.asInteger(pi) - 1);
				if (downgj > g[i]) {
					downgj = g[i]; // ‘Bridge’s Lgth.’
				}
				averagexj += O.X[i];
			}
			averagexj /= Mj.size();

			double _1_ = universalCodeLength(Mj.size()); // Cardinality
			double _2_ = universalCodeLength(n); // Nearest inlier
			double _3_ = t * universalCodeLength((int) (downgj / R[0])); // ‘Bridge’s Length’
			double _4_ = t * universalCodeLength((int) (1 + FastMath.ceil(averagexj / R[0]))); // Average 1NN Dist.
			final double sj = (_1_ + _2_ + _3_ + (Mj.size() - 1) * _4_) / Mj.size();
			Mj.s = sj; // Set of scores per mc
		}
		// END: Compute the scores per microcluster

		// BEGIN: Compute the scores per point
		WritableDoubleDataStore W = DataStoreUtil.makeDoubleStorage(P.getDBIDs(), DataStoreFactory.HINT_HOT);
		for (DBIDIter pi = P.iterDBIDs(); pi.valid(); pi.advance()) {
			int i = (DBIDUtil.asInteger(pi) - 1);
			final double wi = universalCodeLength((int) (1 + g[i] / R[0]));
			W.put(pi, wi);
		}
		// END: Compute the scores per point
		return W;
	}

	protected static double universalCodeLength(int x) {
		double inner = x;
		double result = 0.;
		while (inner > 0) {
			inner = FastMath.log(2., inner);
			if (inner > 0)
				result += inner;
		}
		return result;
	}

	protected int MDL(final int[] sequence) {
		int cutPoint = -1;
		double preAverage, postAverage, descriptionLength, minimumDescriptionLength = 0.;
		for (int i = 0; i < sequence.length; i++) {
			descriptionLength = 0;
			// calculates the average of both sets
			preAverage = 0;
			for (int j = 0; j < i; j++) {
				preAverage += sequence[j];
			} // end for
			if (i != 0) {
				preAverage /= i;
				descriptionLength += (FastMath.ceil(preAverage) != 0.)
						? (FastMath.log10(FastMath.ceil(preAverage)) / FastMath.log10((double) 2))
						: 0; // changes the log base from 10
				// to 2
			} // end if
			postAverage = 0;
			for (int j = i; j < sequence.length; j++) {
				postAverage += sequence[j];
			} // end for
			if ((sequence.length - i) != 0) {
				postAverage /= (sequence.length - i);
				descriptionLength += (FastMath.ceil(postAverage) != 0.)
						? (FastMath.log10(FastMath.ceil(postAverage)) / FastMath.log10((double) 2))
						: 0; // changes the log base from
				// 10 to 2
			} // end if
				// calculates the description length
			for (int j = 0; j < i; j++) {
				descriptionLength += (FastMath.ceil(FastMath.abs(preAverage - sequence[j])) != 0.)
						? (FastMath.log10(FastMath.ceil(FastMath.abs(preAverage - sequence[j]))) / FastMath.log10((double) 2))
						: 0; // changes the log base from 10 to 2
			} // end for
			for (int j = i; j < sequence.length; j++) {
				descriptionLength += (FastMath.ceil(FastMath.abs(postAverage - sequence[j])) != 0.)
						? (FastMath.log10(FastMath.ceil(FastMath.abs(postAverage - sequence[j]))) / FastMath.log10((double) 2))
						: 0; // changes the log base from 10 to 2
			} // end for
				// verify if this is the best cut point
			if (cutPoint == -1 || descriptionLength < minimumDescriptionLength) {
				cutPoint = i;
				minimumDescriptionLength = descriptionLength;
			} // end if
		} // end for

		return cutPoint;
	}

	private static class OPlot {
		protected double[] X;
		protected double[] Y;

		/**
		 * Default constructor
		 */
		public OPlot(int n) {
			X = new double[n];
			Y = new double[n];
			Arrays.fill(X, 0.);
			Arrays.fill(Y, 0.);
		}
	}

	private static class MicroCluster {
		/**
		 * Members ids.
		 */
		protected ArrayModifiableDBIDs members;

		/**
		 * Score.
		 */
		protected double s = 0.;

		/**
		 * Default constructor.
		 */

		public MicroCluster() {

		}

		/**
		 * Add member at microcluster.
		 * 
		 * @return
		 */
		public void addMember(DBIDRef id) {
			if (Objects.isNull(members)) {
				members = DBIDUtil.newArray();
			}
			members.add(id);
		}

		public int size() {
			return members.size();
		}
	}

	public static class Par<O> implements Parameterizer {
		/**
		 * Parameter for number of radii.
		 */
		public static final OptionID A_ID = new OptionID("mccatch.a", "a");

		/**
		 * Parameter for maximum plateau slope.
		 */
		public static final OptionID B_ID = new OptionID("mccatch.b", "b");

		/**
		 * Parameter for maximum microcluster cardinality.
		 */
		public static final OptionID C_ID = new OptionID("mccatch.c", "c");

		/**
		 * Parameter for the type of output to choose.
		 */
		public static final OptionID OUTPUT_ID = new OptionID("mccatch.output", "output");

		/**
		 * Number of radiuses.
		 */
		protected int a = 15;

		/**
		 * Maximum plateau slope.
		 */
		protected double b = 0.1;

		/**
		 * Maximum microcluster size.
		 */
		protected double c = 0.1;

		/**
		 * The distance function to use.
		 */
		protected Distance<? super O> distance;

		/**
		 * Output type to choose.
		 */
		protected int output;

		@Override
		public void configure(Parameterization config) {
			new IntParameter(A_ID, 15) //
					.addConstraint(CommonConstraints.GREATER_THAN_ONE_INT) //
					.grab(config, x -> a = x);
			new DoubleParameter(B_ID, 0.1) //
					.addConstraint(CommonConstraints.GREATER_EQUAL_ZERO_DOUBLE) //
					.grab(config, x -> b = x);
			new DoubleParameter(C_ID, 0.1) //
					.addConstraint(CommonConstraints.GREATER_THAN_ZERO_DOUBLE) //
					.grab(config, x -> c = x);
			new ObjectParameter<Distance<? super O>>(Algorithm.Utils.DISTANCE_FUNCTION_ID, Distance.class, EuclideanDistance.class) //
					.grab(config, x -> distance = x);
			new IntParameter(OUTPUT_ID, 0) //
					.addConstraint(CommonConstraints.GREATER_EQUAL_ZERO_INT) //
					.grab(config, x -> output = x);
		}

		@Override
		public McCatch<?> make() {
			return new McCatch<>(a, b, c, distance, output);
		}
	}

}
