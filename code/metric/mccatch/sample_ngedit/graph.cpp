//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------
#ifndef objectH
#define objectH

#include <cstdlib>
#include <cstdint>
#include <math.h>
#include <string>
#include <time.h>
#include <ostream>
#include <vector>
#include <iostream>

#pragma hdrstop
#pragma package(smart_init)

using namespace std;

// Metric Tree includes
#include <arboretum/stUtil.h>

#include <hermes/DistanceFunction.h>

#include "Application.h"
#include <unordered_set>

//---------------------------------------------------------------------------
// Class TGraph
//---------------------------------------------------------------------------
/**
 * This interface requires no inheritance (because of the use of class
 * templates in the Structure Layer) but requires the following methods:
 *     - TGraph() - A default constructor.
 *     - Clone() - Creates a clone of this object.
 *     - IsEqual() - Checks if this instance is equal to another.
 *     - GetSerializedSize() - Gets the size of the serialized version of this object.
 *     - Serialize() - Gets the serialzied version of this object.
 *     - Unserialize() - Restores a serialzied object.
 *
 * <P>Since the array which contains the serialized version of the object must be
 * created and destroyed by each object instance, this class will hold this array
 * as a buffer of the serialized version of this instance. This buffer will be
 * created only if required and will be invalidated every time the object changes
 * its values.
 *
 * <P>The serialized version of the object will be created as follows:<BR>
 * <CODE>
 * +-----+--------+<BR>
 * | OID | Edges[] |<BR>
 * +-----+--------+<BR>
 * </CODE>
 *
 * @version 1.0
 */
class TGraph
{
public:
   /**
    * Default constructor.
    */
   TGraph()
   {
      ID = 0;
      Edges.clear();

      // Invalidate Serialized buffer.
      Serialized = NULL;
   } // end TName

   /**
    * Creates a new Name object.
    */
   TGraph(long id, const vector<vector<int>> edges)
   {
      ID = id;
      Edges = edges;

      // Invalidate Serialized buffer.
      Serialized = NULL;
   } // end TName

   /**
    * Destroys this instance and releases all associated resources.
    */
   ~TGraph()
   {
      // Does Serialized exist ?
      if (Serialized != NULL)
      {
         // Yes! Dispose it!
         delete[] Serialized;
      } // end if
   }    // end TName

   /**
    * Gets the name.
    */
   const vector<vector<int>> &GetEdges()
   {
        return Edges;
   } // end GetEdges

   /**
    * Gets the ID.
    */
   long GetID()
   {
      return ID;
   } // end GetID

   // The following methods are required by the stObject interface.
   /**
    * Creates a perfect clone of this object. This method is required by
    * stObject interface.
    *
    * @return A new instance of TGraph wich is a perfect clone of the original
    * instance.
    */
   TGraph *Clone()
   {
      return new TGraph(ID, Edges);
   } // end Clone

   /**
    * Checks to see if this object is equal to other. This method is required
    * by  stObject interface.
    *
    * @param obj Another instance of TName.
    * @return True if they are equal or false otherwise.
    */
   bool IsEqual(TGraph *obj)
   {
      return (Edges == obj->GetEdges());
   } // end IsEqual

   /**
    * Returns the size of the serialized version of this object in bytes.
    * This method is required  by  stObject interface.
    */
   size_t GetSerializedSize()
   {
      size_t buffer_size = sizeof(ID) + sizeof(int) * 2 + sizeof(int) * Edges.size() * 2;
      for (const auto& vec : Edges) {
         buffer_size += sizeof(int) * vec.size();
      }
      return buffer_size;
   } // end GetSerializedSize

   /**
    * Returns the serialized version of this object.
    * This method is required  by  stObject interface.
    *
    * @warning If you don't know how to serialize an object, this methos may
    * be a good example.
    */
   const uint8_t *Serialize()
   {
      // Is there a seralized version ?
      if (Serialized == NULL)
      {
        // Allocate the buffer
        Serialized = new uint8_t[GetSerializedSize()];
        uint8_t* p = Serialized;         

        // Serialize the data
        memcpy(p, &ID, sizeof(ID));
        p += sizeof(ID);
        int num_rows = Edges.size();
        memcpy(p, &num_rows, sizeof(num_rows));
        p += sizeof(num_rows);
        int num_cols = 0;
        for (const auto& vec : Edges) {
            num_cols += vec.size();
        }
        memcpy(p, &num_cols, sizeof(num_cols));
        p += sizeof(num_cols);
        for (const auto& vec : Edges) {
            int vec_size = vec.size();
            memcpy(p, &vec_size, sizeof(vec_size));
            p += sizeof(vec_size);
            memcpy(p, vec.data(), sizeof(int) * vec_size);
            p += sizeof(int) * vec_size;
        }      } // end if

      return Serialized;
   } // end Serialize

   /**
    * Rebuilds a serialized object.
    * This method is required  by  stObject interface.
    *
    * @param data The serialized object.
    * @param datasize The size of the serialized object in bytes.
    * @warning If you don't know how to serialize an object, this methos may
    * be a good example.
    */
   void Unserialize(const uint8_t *data, size_t datasize)
   {
      // Deserialize the data
      ID = *reinterpret_cast<const long*>(data);
      data += sizeof(ID);
      int num_rows = *reinterpret_cast<const int*>(data);
      data += sizeof(num_rows);
      int num_cols = *reinterpret_cast<const int*>(data);
      data += sizeof(num_cols);
      vector<vector<int>> edges(num_rows);
      for (auto& vec : edges) {
         int vec_size = *reinterpret_cast<const int*>(data);
         data += sizeof(vec_size);
         vec.resize(vec_size);
         memcpy(vec.data(), data, sizeof(int) * vec_size);
         data += sizeof(int) * vec_size;
      }
      Edges = edges;
      // Since we have changed the object contents, we must invalidate the old
      // serialized version if it exists. In fact we, may copy the given serialized
      // version of tbe new object to the buffer but we don't want to spend memory.
      if (Serialized != NULL)
      {
         delete[] Serialized;
         Serialized = NULL;
      } // end if
   }    // end Unserialize

   long GetOID()
   {
      return GetID();
   } // end GetOID()

private:
   /**
    * ID.
    */
   long ID;

   vector<vector<int>> Edges;

   /**
    * Serialized version. If NULL, the serialized version is not created.
    */
   uint8_t *Serialized;
}; // end TMapPoint

//---------------------------------------------------------------------------
// Class TGraphDistanceEvaluator
//---------------------------------------------------------------------------
/**
 * This class implements a metric evaluator for TGraph instances. It calculates
 * the distance between names by performing a Levenshtein distance.
 *
 * <P>It implements the stMetricEvaluator interface. As stObject interface, the
 * stMetricEvaluator interface requires no inheritance and defines 2 methods:
 *     - GetDistance() - Calculates the distance between 2 objects.
 *     - GetLEditDistance()  - Calculates the graph edit distance.
 *
 * @version 1.0
 */
class TGraphDistanceEvaluator : public DistanceFunction<TGraph>
{
private:
    const int INF = 1e9;
public:
   TGraphDistanceEvaluator()
   {
   }

   /**
    * Returns the distance between 2 objects. This method is required by
    * stMetricEvaluator interface.
    *
    * @param obj1 Object 1.
    * @param obj2 Object 2.
    */
   double GetDistance(TGraph &obj1, TGraph &obj2)
   {
      //return GetGEditDistance(obj1, obj2);
      return GetNGEditDistance(obj1, obj2);
   } // end GetDistance

   double getDistance(TGraph &obj1, TGraph &obj2)
   {
      //return GetGEditDistance(obj1, obj2);
      return GetNGEditDistance(obj1, obj2);
   }

   /**
    * Returns the graph edit distance A*-based
    *
    * @param obj1 Object 1.
    * @param obj2 Object 2.
    */
   double GetGEditDistance(TGraph &obj1, TGraph &obj2)
   {
      vector<pair<int,ui> > vertices;
      unordered_set<int> _vertices;
      vector<pair<pair<int,int>,ui> > edges;

      for(const auto& edge: obj1.GetEdges()){
         for(const auto& vertex: edge){
            if(!_vertices.count(vertex)){
               _vertices.insert(vertex);
               vertices.push_back(make_pair(vertex, 0));
            }
         }
         edges.push_back(make_pair(make_pair(edge[0], edge[1]), 0));
         edges.push_back(make_pair(make_pair(edge[1], edge[0]), 0));
      }
      sort(vertices.begin(), vertices.end());
      sort(edges.begin(), edges.end());
      Graph *graph1 = new Graph("1", vertices, edges);

      vector<pair<int,ui> >().swap(vertices);
      unordered_set<int>().swap(_vertices);
      vector<pair<pair<int,int>,ui> >().swap(edges);

      for(const auto& edge: obj2.GetEdges()){
         for(const auto& vertex: edge){
            if(!_vertices.count(vertex)){
               _vertices.insert(vertex);
               vertices.push_back(make_pair(vertex, 0));
            }
         }
         edges.push_back(make_pair(make_pair(edge[0], edge[1]), 0));
         edges.push_back(make_pair(make_pair(edge[1], edge[0]), 0));
      }
      sort(vertices.begin(), vertices.end());
      sort(edges.begin(), edges.end());
      Graph *graph2 = new Graph("2", vertices, edges);

      vector<pair<int,ui> >().swap(vertices);
      unordered_set<int>().swap(_vertices);
      vector<pair<pair<int,int>,ui> >().swap(edges);

      Application *app = new Application(INF, "BMao");
      app->init(graph1, graph2);

      double ged = app->AStar();
      updateDistanceCount();
      
      delete app;
      delete graph1;
      delete graph2;

      return ged;
   } // end GetGEditDistance

   /**
    * Returns the Normalized Generalized Graph Edit distance
    * https://link.springer.com/article/10.1007/s11704-011-9336-2
    *
    * @param obj1 Object 1.
    * @param obj2 Object 2.
    */
   double GetNGEditDistance(TGraph &obj1, TGraph &obj2)
   {
      vector<vector<int>> G1 = obj1.GetEdges(), G2 = obj2.GetEdges();
      double GLD = GetGEditDistance(obj1, obj2);
      double alpha = 1.;

      return (2 * GLD) / (alpha * ((G1.size()+1) + (G2.size()+1)) + GLD);
   } // end GetNGEditDistance

   //---------------------------------------------------------------------------
   // Output operator
   //---------------------------------------------------------------------------
   /**
    * This operator will write a string representation of a city to an outputstream.
    */
   friend ostream &operator<<(ostream &out, TGraph &name)
   {
      out << "[OID=" << name.GetID() << ";Name=" << "-" << "]";
      return out;
   } // end operator <<

}; // end TGraphDistanceEvaluator

#endif // end myobjectH
