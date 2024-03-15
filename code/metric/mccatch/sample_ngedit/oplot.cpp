//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------
#ifndef oplotH
#define oplotH

// #pragma hdrstop
// #pragma package(smart_init)

//---------------------------------------------------------------------------
// Class OPlot
//---------------------------------------------------------------------------
/**
 * <CODE>
 * +-----+-----+<BR>
 * | X[] | Y[] |<BR>
 * +-----+----+<BR>
 * </CODE>
 *
 * @version 1.0
 */
class OPlot
{
public:
    /**
     * Default constructor.
     */
    OPlot()
    {

    } // end TName

    /**
     * Creates a new OPlot object.
     */
    OPlot(int n)
    {
        X = new double[n];
        Y = new double[n];
    } // end TName

    /**
     * Destroys this instance and releases all associated resources.
     */
    ~OPlot()
    {
        // Does Serialized exist ?
        if (X != NULL)
        {
            // Yes! Dispose it!
            delete[] X;
        } // end if

        if (Y != NULL)
        {
            delete[] Y;
        }
    } // end OPlot

    /**
     * Get X.
     */
    double *GetX()
    {
        return X;
    } // end GetX

    /**
     * Get X.
     */
    double *GetY()
    {
        return Y;
    } // end GetY

    // The following methods are required by the stObject interface.
    /**
     * Creates a perfect clone of this object. This method is required by
     * stObject interface.
     *
     * @return A new instance of TName wich is a perfect clone of the original
     * instance.
     */
    OPlot *Clone()
    {
        return new OPlot(sizeof(X) / sizeof(double));
    } // end Clone

private:
    double *X;
    double *Y;

}; // end OPlot

#endif // end oplotH