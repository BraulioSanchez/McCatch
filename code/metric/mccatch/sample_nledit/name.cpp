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

//---------------------------------------------------------------------------
// Class TName
//---------------------------------------------------------------------------
/**
 * This interface requires no inheritance (because of the use of class
 * templates in the Structure Layer) but requires the following methods:
 *     - TName() - A default constructor.
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
 * | OID | Name[] |<BR>
 * +-----+--------+<BR>
 * </CODE>
 *
 * @version 1.0
 */
class TName
{
public:
   /**
    * Default constructor.
    */
   TName()
   {
      ID = 0;
      Name = "";

      // Invalidate Serialized buffer.
      Serialized = NULL;
   } // end TName

   /**
    * Creates a new Name object.
    *
    * @param id ID.
    * @param name The name.
    */
   TName(long id, const string name)
   {
      ID = id;
      Name = name;

      // Invalidate Serialized buffer.
      Serialized = NULL;
   } // end TName

   /**
    * Destroys this instance and releases all associated resources.
    */
   ~TName()
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
   const string &GetName()
   {
      return Name;
   } // end GetName

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
    * @return A new instance of TName wich is a perfect clone of the original
    * instance.
    */
   TName *Clone()
   {
      return new TName(ID, Name);
   } // end Clone

   /**
    * Checks to see if this object is equal to other. This method is required
    * by  stObject interface.
    *
    * @param obj Another instance of TName.
    * @return True if they are equal or false otherwise.
    */
   bool IsEqual(TName *obj)
   {
      return (Name == obj->GetName());
   } // end IsEqual

   /**
    * Returns the size of the serialized version of this object in bytes.
    * This method is required  by  stObject interface.
    */
   size_t GetSerializedSize()
   {
      return sizeof(long) + Name.length();
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
      long *d;

      // Is there a seralized version ?
      if (Serialized == NULL)
      {
         // No! Lets build the serialized version.

         // The first thing we need to do is to allocate resources...
         Serialized = new uint8_t[GetSerializedSize()];

         // We will organize it in this manner:
         // +-----+--------+
         // | OID | Name[] |
         // +-----+--------+
         d = (long *)Serialized;
         d[0] = ID;

         memcpy(Serialized + sizeof(long), Name.c_str(), Name.length());
      } // end if

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
      long *d;
      size_t strl;

      // This is the reverse of Serialize(). So the steps are similar.
      // Remember, the format of the serizalized object is
      // +-----+--------+
      // | OID | Name[] |
      // +-----+--------+

      d = (long *)data; // If you ar not familiar with pointers, this
                        // action may be tricky! Be careful!
      ID = d[0];

      // To read the name, we must discover its size first. Since it is the only
      // variable length field, we can get it back by subtract the fixed size
      // from the serialized size.
      strl = datasize - sizeof(long);

      // Now we know the size, lets get it from the serialized version.
      Name.assign((char *)(data + sizeof(long)), strl);

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

   /**
    * Name.
    */
   string Name;

   /**
    * Serialized version. If NULL, the serialized version is not created.
    */
   uint8_t *Serialized;
}; // end TMapPoint

//---------------------------------------------------------------------------
// Class TNameDistanceEvaluator
//---------------------------------------------------------------------------
/**
 * This class implements a metric evaluator for TName instances. It calculates
 * the distance between names by performing a Levenshtein distance.
 *
 * <P>It implements the stMetricEvaluator interface. As stObject interface, the
 * stMetricEvaluator interface requires no inheritance and defines 2 methods:
 *     - GetDistance() - Calculates the distance between 2 objects.
 *     - GetLEditDistance()  - Calculates the Levenshtein distance.
 *
 * @version 1.0
 */
class TNameDistanceEvaluator : public DistanceFunction<TName>
{
public:
   TNameDistanceEvaluator()
   {
   }

   /**
    * Returns the distance between 2 objects. This method is required by
    * stMetricEvaluator interface.
    *
    * @param obj1 Object 1.
    * @param obj2 Object 2.
    */
   double GetDistance(TName &obj1, TName &obj2)
   {
      // return GetLEditDistance(obj1, obj2);
      return GetNLEditDistance(obj1, obj2);
   } // end GetDistance

   double getDistance(TName &obj1, TName &obj2)
   {
      // return GetLEditDistance(obj1, obj2);
      return GetNLEditDistance(obj1, obj2);
   }

   /**
    * Returns the Levenshtein distance
    *
    * @param obj1 Object 1.
    * @param obj2 Object 2.
    */
   double GetLEditDistance(TName &obj1, TName &obj2)
   {
      string S = obj1.GetName();
      string T = obj2.GetName();
      const int N = (int)S.size(), M = (int)T.size();
      vector<vector<int>> dp(N + 1, vector<int>(M + 1, N + M));
      for (int i = 0; i <= N; i++)
         dp[i][0] = i;
      for (int i = 0; i <= M; i++)
         dp[0][i] = i;
      for (int i = 1; i <= N; i++)
      {
         for (int j = 1; j <= M; j++)
         {
            dp[i][j] = min(dp[i][j], dp[i - 1][j] + 1);
            dp[i][j] = min(dp[i][j], dp[i][j - 1] + 1);
            dp[i][j] = min(dp[i][j], dp[i - 1][j - 1] + (S[i - 1] != T[j - 1]));
         }
      }
      updateDistanceCount();

      return dp[N][M];
   } // end GetLEditDistance

   /**
    * Returns the Normalized Generalized Levenshtein distance
    * https://ieeexplore.ieee.org/document/4160958
    *
    * @param obj1 Object 1.
    * @param obj2 Object 2.
    */
   double GetNLEditDistance(TName &obj1, TName &obj2)
   {
      string S = obj1.GetName();
      string T = obj2.GetName();
      double GLD = GetLEditDistance(obj1, obj2);
      double alpha = 1.;

      return (2 * GLD) / (alpha * (S.size() + T.size()) + GLD);
   } // end GetNLEditDistance

   //---------------------------------------------------------------------------
   // Output operator
   //---------------------------------------------------------------------------
   /**
    * This operator will write a string representation of a city to an outputstream.
    */
   friend ostream &operator<<(ostream &out, TName &name)
   {
      out << "[OID=" << name.GetID() << ";Name=" << name.GetName() << "]";
      return out;
   } // end operator <<

}; // end TNameDistanceEvaluator

#endif // end myobjectH