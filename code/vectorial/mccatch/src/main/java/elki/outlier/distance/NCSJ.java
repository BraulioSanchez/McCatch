package elki.outlier.distance;

import java.util.Arrays;

import elki.data.DoubleVector;
import elki.data.ModifiableHyperBoundingBox;
import elki.data.NumberVector;
import elki.data.spatial.SpatialUtil;
import elki.database.ids.DBIDUtil;
import elki.database.relation.Relation;
import elki.distance.Distance;
import elki.distance.SpatialPrimitiveDistance;
import elki.index.tree.spatial.SpatialDirectoryEntry;
import elki.index.tree.spatial.SpatialEntry;
import elki.index.tree.spatial.SpatialPointLeafEntry;
import elki.index.tree.spatial.rstarvariants.AbstractRStarTree;
import elki.index.tree.spatial.rstarvariants.AbstractRStarTreeNode;
import it.unimi.dsi.fastutil.ints.IntArrayList;

/**
 *
 * @param <V>
 */
public class NCSJ<O extends NumberVector> {

	private boolean selfJoin;
	private boolean allowSelfSimilar = false;
	private SpatialPrimitiveDistance<? super O> distanceFunction;

	public NCSJ() {
		this.allowSelfSimilar = true;
	}

	public NCSJ(boolean allowSelfSimilar) {
		this.allowSelfSimilar = allowSelfSimilar;
	}

	public void simJoin(Relation<O> relation, AbstractRStarTree<?, ?, ?> tree, Distance<? super O> distance, final int[] neighbors,
			final double scale) {
		this.distanceFunction = (SpatialPrimitiveDistance<? super O>) distance;
		selfJoin = true;
		simJoin(tree.getRootID(), tree.getRootEntry(), tree, neighbors, scale);
		selfJoin = false;
		this.distanceFunction = null;
	}

	public void simJoin(Relation<O> relation, AbstractRStarTree<?, ?, ?> tree1, AbstractRStarTree<?, ?, ?> tree2,
			Distance<? super O> distance, final int[] neighbors, final double scale) {
		this.distanceFunction = (SpatialPrimitiveDistance<? super O>) distance;
		selfJoin = false;
		simJoin(tree1.getRootID(), tree1.getRootEntry(), tree1, tree2.getRootID(), tree2.getRootEntry(), tree2, neighbors, scale);
		this.distanceFunction = null;
	}

	private void simJoin(final int pageID, final SpatialEntry spatialEntry, AbstractRStarTree<?, ?, ?> tree, final int[] neighbors,
			final double scale) {
		// Processing queue.
		int[] pq = new int[101];
		int ps = 0;
		pq[ps] = pageID;

		SpatialEntry[] _pq = new SpatialEntry[101];
		_pq[ps++] = spatialEntry;

		// search in tree
		while (ps > 0) {
			int pqNode = pq[--ps]; // Pop last.
			AbstractRStarTreeNode<?, ?> node = tree.getNode(pqNode);
			SpatialEntry entry = _pq[ps];
			final int numEntries = node.getNumEntries();

			if (maximumDiameter(entry) <= scale) {
				createNewGroup(node.getPageID(), entry, tree, neighbors);
				continue;
			}

			if (node.isLeaf()) {
				for (int i = 0; i < numEntries; i++) {
					SpatialPointLeafEntry e1 = (SpatialPointLeafEntry) node.getEntry(i);
					for (int j = 0; j < numEntries; j++) {
						SpatialPointLeafEntry e2 = (SpatialPointLeafEntry) node.getEntry(j);

						if (!allowSelfSimilar && DBIDUtil.equal(e1.getDBID(), e2.getDBID())) {
							continue;
						}

						double distance = this.distanceFunction.minDist(e1, e2);
						if (distance <= scale) {
							neighbors[DBIDUtil.asInteger(e1.getDBID()) - 1] += 1;
						}
					}
				}
			} else {
				for (int i = 0; i < numEntries; i++) {
					SpatialDirectoryEntry a = (SpatialDirectoryEntry) node.getEntry(i);
					if (ps == pq.length) {
						pq = Arrays.copyOf(pq, pq.length + (pq.length >>> 1));
						_pq = Arrays.copyOf(_pq, _pq.length + (_pq.length >>> 1));
					}
					pq[ps] = a.getPageID();
					_pq[ps++] = a;
					for (int j = i + 1; j < numEntries; j++) {
						SpatialDirectoryEntry a_ = (SpatialDirectoryEntry) node.getEntry(j);
						simJoin(a.getPageID(), a, tree, a_.getPageID(), a_, tree, neighbors, scale);
					}
				}
			}
		}
	}

	private void simJoin(final int pageID1, final SpatialEntry spatialEntry1, AbstractRStarTree<?, ?, ?> tree1, final int pageID2,
			final SpatialEntry spatialEntry2, AbstractRStarTree<?, ?, ?> tree2, final int[] neighbors, final double scale) {
		// Processing queue.
		int[] pq1 = new int[101];
		int ps1 = 0;
		pq1[ps1] = pageID1;
		SpatialEntry[] _pq1 = new SpatialEntry[101];
		_pq1[ps1++] = spatialEntry1;

		int[] pq2 = new int[101];
		int ps2 = 0;
		pq2[ps2] = pageID2;
		SpatialEntry[] _pq2 = new SpatialEntry[101];
		_pq2[ps2++] = spatialEntry2;

		// search in tree
		while (ps1 > 0 && ps2 > 0) {
			int pqNode1 = pq1[--ps1]; // Pop last.
			int pqNode2 = pq2[--ps2]; // Pop last.
			AbstractRStarTreeNode<?, ?> node1 = tree1.getNode(pqNode1);
			AbstractRStarTreeNode<?, ?> node2 = tree2.getNode(pqNode2);
			SpatialEntry entry1 = _pq1[ps1];
			SpatialEntry entry2 = _pq2[ps2];
			final int numEntries1 = node1.getNumEntries();
			final int numEntries2 = node2.getNumEntries();

			if (maximumDiameter(entry1, entry2) <= scale) {
				createNewGroup(node1.getPageID(), entry1, tree1, node2.getPageID(), entry2, tree2, neighbors);
				continue;
			}

			if (this.distanceFunction.minDist(entry1, entry2) > scale) {
				continue;
			}

			if (node1.isLeaf() && node2.isLeaf()) {
				for (int i = 0; i < numEntries1; i++) {
					SpatialPointLeafEntry o1 = (SpatialPointLeafEntry) node1.getEntry(i);
					for (int j = 0; j < numEntries2; j++) {
						SpatialPointLeafEntry o2 = (SpatialPointLeafEntry) node2.getEntry(j);

						if (!allowSelfSimilar && DBIDUtil.equal(o1.getDBID(), o2.getDBID())) {
							continue;
						}

						double distance = this.distanceFunction.minDist(o1, o2);
						if (distance <= scale) {
							neighbors[DBIDUtil.asInteger(o1.getDBID()) - 1] += 1;
							if (selfJoin) {
								neighbors[DBIDUtil.asInteger(o2.getDBID()) - 1] += 1;
							}
						}
					}
				}
			} else if (node1.isLeaf()) {
				for (int i = 0; i < numEntries2; i++) {
					SpatialDirectoryEntry c2 = (SpatialDirectoryEntry) node2.getEntry(i);
					if (ps1 == pq1.length) {
						pq1 = Arrays.copyOf(pq1, pq1.length + (pq1.length >>> 1));
						_pq1 = Arrays.copyOf(_pq1, _pq1.length + (_pq1.length >>> 1));
					}
					if (ps2 == pq2.length) {
						pq2 = Arrays.copyOf(pq2, pq2.length + (pq2.length >>> 1));
						_pq2 = Arrays.copyOf(_pq2, _pq2.length + (_pq2.length >>> 1));
					}
					pq1[ps1] = node1.getPageID();
					_pq1[ps1++] = entry1;
					pq2[ps2] = c2.getPageID();
					_pq2[ps2++] = c2;
				}
			} else if (node2.isLeaf()) {
				for (int i = 0; i < numEntries1; i++) {
					SpatialDirectoryEntry c1 = (SpatialDirectoryEntry) node1.getEntry(i);
					if (ps1 == pq1.length) {
						pq1 = Arrays.copyOf(pq1, pq1.length + (pq1.length >>> 1));
						_pq1 = Arrays.copyOf(_pq1, _pq1.length + (_pq1.length >>> 1));
					}
					if (ps2 == pq2.length) {
						pq2 = Arrays.copyOf(pq2, pq2.length + (pq2.length >>> 1));
						_pq2 = Arrays.copyOf(_pq2, _pq2.length + (_pq2.length >>> 1));
					}
					pq1[ps1] = c1.getPageID();
					_pq1[ps1++] = c1;
					pq2[ps2] = node2.getPageID();
					_pq2[ps2++] = entry2;
				}

			} else {
				for (int i = 0; i < numEntries1; i++) {
					SpatialDirectoryEntry c1 = (SpatialDirectoryEntry) node1.getEntry(i);
					for (int j = 0; j < numEntries2; j++) {
						SpatialDirectoryEntry c2 = (SpatialDirectoryEntry) node2.getEntry(j);
						if (ps1 == pq1.length) {
							pq1 = Arrays.copyOf(pq1, pq1.length + (pq1.length >>> 1));
							_pq1 = Arrays.copyOf(_pq1, _pq1.length + (_pq1.length >>> 1));
						}
						if (ps2 == pq2.length) {
							pq2 = Arrays.copyOf(pq2, pq2.length + (pq2.length >>> 1));
							_pq2 = Arrays.copyOf(_pq2, _pq2.length + (_pq2.length >>> 1));
						}
						pq1[ps1] = c1.getPageID();
						_pq1[ps1++] = c1;
						pq2[ps2] = c2.getPageID();
						_pq2[ps2++] = c2;
					}
				}
			}
		}
	}

	@SuppressWarnings("unchecked")
	private double maximumDiameter(SpatialEntry spatialEntry) {
		ModifiableHyperBoundingBox mbr = new ModifiableHyperBoundingBox(spatialEntry);
		O vMax = (O) new DoubleVector(mbr.getMaxRef());
		O vMin = (O) new DoubleVector(mbr.getMinRef());
		return distanceFunction.distance(vMin, vMax);
	}

	@SuppressWarnings("unchecked")
	private double maximumDiameter(SpatialEntry spatialEntry1, SpatialEntry spatialEntry2) {
		ModifiableHyperBoundingBox mbr = SpatialUtil.union(spatialEntry1, spatialEntry2);
		O vMax = (O) new DoubleVector(mbr.getMaxRef());
		O vMin = (O) new DoubleVector(mbr.getMinRef());
		return distanceFunction.distance(vMin, vMax);
	}

	private void createNewGroup(final int pageID, SpatialEntry spatialEntry, AbstractRStarTree<?, ?, ?> tree, final int[] neighbors) {
		final IntArrayList intDBIDs = new IntArrayList();
		countLeafEntries(pageID, tree, intDBIDs);
		int countLeafEntries = intDBIDs.size();

		if (!allowSelfSimilar) {
			countLeafEntries -= 1;
		}

		for (int intDBID : intDBIDs) {
			neighbors[intDBID] += countLeafEntries;
		}
	}

	private void createNewGroup(final int pageID1, SpatialEntry spatialEntry1, AbstractRStarTree<?, ?, ?> tree1, final int pageID2,
			SpatialEntry spatialEntry2, AbstractRStarTree<?, ?, ?> tree2, final int[] neighbors) {
		final IntArrayList intDBIDs1 = new IntArrayList();
		countLeafEntries(pageID1, tree1, intDBIDs1);
		int countLeafEntries1 = intDBIDs1.size();

		final IntArrayList intDBIDs2 = new IntArrayList();
		countLeafEntries(pageID2, tree2, intDBIDs2);
		int countLeafEntries2 = intDBIDs2.size();

		for (int intDBID : intDBIDs1) {
			neighbors[intDBID] += countLeafEntries2;
		}

		if (selfJoin) {
			for (int intDBID : intDBIDs2) {
				neighbors[intDBID] += countLeafEntries1;
			}
		}
	}

	private void countLeafEntries(int pageID, AbstractRStarTree<?, ?, ?> tree, IntArrayList intDBIDs) {
		// Processing queue.
		int[] pq = new int[101];
		int ps = 0;
		pq[ps++] = pageID;

		// search in node
		while (ps > 0) {
			int pqNode = pq[--ps]; // Pop last.
			AbstractRStarTreeNode<?, ?> node = tree.getNode(pqNode);
			final int numEntries = node.getNumEntries();

			if (node.isLeaf()) {
				for (int i = 0; i < numEntries; i++) {
					SpatialPointLeafEntry leafEntry = (SpatialPointLeafEntry) node.getEntry(i);
					intDBIDs.add(DBIDUtil.asInteger(leafEntry.getDBID()) - 1);
				}
			} else {
				for (int i = 0; i < numEntries; i++) {
					SpatialDirectoryEntry entry = (SpatialDirectoryEntry) node.getEntry(i);
					if (ps == pq.length) {
						pq = Arrays.copyOf(pq, pq.length + (pq.length >>> 1));
					}
					pq[ps++] = entry.getPageID();
				}
			}
		}
	}

}
