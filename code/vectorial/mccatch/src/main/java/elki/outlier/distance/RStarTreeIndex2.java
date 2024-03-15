package elki.outlier.distance;

import java.util.ArrayList;
import java.util.List;

import elki.data.NumberVector;
import elki.database.ids.DBIDIter;
import elki.database.ids.DBIDRef;
import elki.database.ids.DBIDUtil;
import elki.database.ids.DBIDs;
import elki.database.query.distance.DistanceQuery;
import elki.database.query.distance.SpatialDistanceQuery;
import elki.database.query.knn.KNNSearcher;
import elki.database.query.range.RangeSearcher;
import elki.database.relation.Relation;
import elki.index.DynamicIndex;
import elki.index.KNNIndex;
import elki.index.RangeIndex;
import elki.index.tree.IndexTreePath;
import elki.index.tree.spatial.SpatialEntry;
import elki.index.tree.spatial.SpatialPointLeafEntry;
import elki.index.tree.spatial.rstarvariants.RTreeSettings;
import elki.index.tree.spatial.rstarvariants.query.RStarTreeUtil;
import elki.index.tree.spatial.rstarvariants.rstar.RStarTree;
import elki.index.tree.spatial.rstarvariants.rstar.RStarTreeNode;
import elki.index.tree.spatial.rstarvariants.strategies.bulk.SortTileRecursiveBulkSplit;
import elki.logging.Logging;
import elki.persistent.MemoryPageFileFactory;
import elki.persistent.PageFile;

public class RStarTreeIndex2<O extends NumberVector> extends RStarTree implements RangeIndex<O>, KNNIndex<O>, DynamicIndex {
	/**
	 * The appropriate logger for this index.
	 */
	private static final Logging LOG = Logging.getLogger(RStarTreeIndex2.class);

	/**
	 * Relation
	 */
	private Relation<O> relation;

	/**
	 * Constructor convenience.
	 * 
	 * @param relation Relation to index
	 * @param pageSize Page file size
	 */
	public RStarTreeIndex2(Relation<O> relation, int pageSize) {
		// settings for build a R*-Tree with bulk loading
//		super(new MemoryPageFileFactory<RStarTreeNode>(pageSize).newPageFile(RStarTreeNode.class), new RTreeSettings() {
//			{
//				setBulkStrategy(SortTileRecursiveBulkSplit.STATIC);
//
//				setInsertionStrategy(new CombinedInsertionStrategy(LeastEnlargementWithAreaInsertionStrategy.STATIC,
//						LeastOverlapInsertionStrategy.STATIC));
//
//				setNodeSplitStrategy(TopologicalSplitter.STATIC);
//
//				setOverflowTreatment(LimitedReinsertOverflowTreatment.RSTAR_OVERFLOW);
//			}
//		});
		super(new MemoryPageFileFactory<RStarTreeNode>(pageSize).newPageFile(RStarTreeNode.class), new RTreeSettings() {
			{
				setBulkStrategy(SortTileRecursiveBulkSplit.STATIC);
			}
		});
		this.relation = relation;
	}

	/**
	 * Constructor.
	 * 
	 * @param relation Relation to index
	 * @param pagefile Page file
	 * @param settings Tree settings
	 */
	public RStarTreeIndex2(Relation<O> relation, PageFile<RStarTreeNode> pagefile, RTreeSettings settings) {
		super(pagefile, settings);
		this.relation = relation;
	}

	/**
	 * Create a new leaf entry.
	 * 
	 * @param id Object id
	 * @return Spatial leaf entry
	 */
	protected SpatialPointLeafEntry createNewLeafEntry(DBIDRef id) {
		return new SpatialPointLeafEntry(DBIDUtil.deref(id), relation.get(id));
	}

	@Override
	public void initialize() {
		super.initialize();
//		insertAll(relation.getDBIDs()); // remove for convenience!
	}

	/**
	 * Inserts the specified reel vector object into this index.
	 * 
	 * @param id the object id that was inserted
	 */
	@Override
	public void insert(DBIDRef id) {
		insertLeaf(createNewLeafEntry(id));
	}

	/**
	 * Inserts the specified objects into this index. If a bulk load mode is
	 * implemented, the objects are inserted in one bulk.
	 * 
	 * @param ids the objects to be inserted
	 */
	@Override
	public void insertAll(DBIDs ids) {
		if (ids.isEmpty()) {
			return;
		}

		// Make an example leaf
		if (canBulkLoad()) {
			List<SpatialEntry> leafs = new ArrayList<>(ids.size());
			for (DBIDIter iter = ids.iter(); iter.valid(); iter.advance()) {
				leafs.add(createNewLeafEntry(iter));
			}
			bulkLoad(leafs);
		} else {
			for (DBIDIter iter = ids.iter(); iter.valid(); iter.advance()) {
				insert(DBIDUtil.deref(iter));
			}
		}

		doExtraIntegrityChecks();
	}

	/**
	 * Deletes the specified object from this index.
	 * 
	 * @return true if this index did contain the object with the specified id,
	 *         false otherwise
	 */
	@Override
	public boolean delete(DBIDRef id) {
		// find the leaf node containing o
		O obj = relation.get(id);
		IndexTreePath<SpatialEntry> deletionPath = findPathToObject(getRootPath(), obj, id);
		if (deletionPath == null) {
			return false;
		}
		deletePath(deletionPath);
		return true;
	}

	@Override
	public void deleteAll(DBIDs ids) {
		for (DBIDIter iter = ids.iter(); iter.valid(); iter.advance()) {
			delete(iter);
		}
	}

	@Override
	protected Logging getLogger() {
		return LOG;
	}

	@Override
	public KNNSearcher<O> kNNByObject(DistanceQuery<O> distanceQuery, int maxk, int flags) {
		// Can we support this distance function - spatial distances only!
		return distanceQuery.getRelation() == relation && distanceQuery instanceof SpatialDistanceQuery ? //
				RStarTreeUtil.getKNNQuery(this, (SpatialDistanceQuery<O>) distanceQuery, maxk, flags) : null;
	}

	@Override
	public RangeSearcher<O> rangeByObject(DistanceQuery<O> distanceQuery, double maxradius, int flags) {
		// Can we support this distance function - spatial distances only!
		return distanceQuery.getRelation() == relation && distanceQuery instanceof SpatialDistanceQuery ? //
				RStarTreeUtil.getRangeQuery(this, (SpatialDistanceQuery<O>) distanceQuery, maxradius, flags) : null;
	}
}