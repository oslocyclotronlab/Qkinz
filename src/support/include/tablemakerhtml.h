

#ifndef TABLEMAKERHTML_H
#define TABLEMAKERHTML_H

#include <QVector>
#include <QString>

//! Class TableMakerHTML
//! A class to take calculated data and
//! creates html file with the data.
class TableMakerHTML
{
public:
    //! To tell what particle.
    enum Particle_t {
        Proton,     //! If proton.
        Deutron,    //! If deutron.
        Triton,     //! If Triton.
        Helium3,    //! If Helium-3.
        Alpha       //! If Alpha.
    };

    //! Constructor.
    TableMakerHTML();

    //! Function to reset all data.
    inline void Reset()
    {
        protons.Reset();
        cProtons.clear();
        deutrons.Reset();
        cDeutrons.clear();
        tritons.Reset();
        cTritons.clear();
        He3s.Reset();
        cHe3s.clear();
        alphas.Reset();
        cAlphas.clear();
    }

    //! Set the calculated data.
    void setData(QVector<double> Ex,    /*!< Excitation energy.                                 */
                 QVector<double> dE,    /*!< Energy deposited in dE detector.                   */
                 QVector<double> d_dE,  /*!< Uncertainty in energy deposited in dE detector.    */
                 QVector<double> E,     /*!< Energy deposited in E detector.                    */
                 QVector<double> d_E,   /*!< Uncertainty in energy deposited in E detector.     */
                 Particle_t what        /*!< Which particle.                                    */);

    //! Set the coefficients to the fitted Ex(e+de) curve.
    void setCoeff(QVector<double> Coeff,    /*!< Vector containing the three coefficients.  */
                  Particle_t what           /*!< Whitch particle.                            */);

    //! Returns the created page as HTML code.
    QString getHTMLCode();

    //! Returns the created page as txt file table.
    QString getTXT();

private:
    //! Internal structure to hold data recived by the class.
    struct Result_t {
        QVector<double> Ex;                 //! Excitation energy.
        QVector<double> dE;                 //! Energy in dE detector.
        QVector<double> d_dE;               //! Uncertainty in energy for dE detector.
        QVector<double> E;                  //! Energy in E detector.
        QVector<double> d_E;                //! Uncertainty in energy for E detector.
        bool is_set;                        //! If the data is set or not.
        Result_t() : is_set( false ){}      //! Constructor.
        void Reset()                        //! Resets the strucure (zeroing all vectors).
        {
            Ex.clear();
            dE.clear();
            d_dE.clear();
            E.clear();
            d_E.clear();
            is_set = false;
        }
    };

    //! Data for protons.
    Result_t protons;

    //! Coeff. for protons.
    QVector<double> cProtons;

    //! Data for deutrons.
    Result_t deutrons;

    //! Coeff. for deutrons.
    QVector<double> cDeutrons;

    //! Data for tritons.
    Result_t tritons;

    //! Coeff. for tritons.
    QVector<double> cTritons;

    //! Data for Helium-3.
    Result_t He3s;

    //! Coeff. for helium-3.
    QVector<double> cHe3s;

    //! Data for alphas.
    Result_t alphas;

    //! Coeff. for alphas.
    QVector<double> cAlphas;

    //! Function to set a strucure with values.
    inline void setResult(QVector<double> Ex,   /*!< Excitation energy.                                 */
                          QVector<double> dE,   /*!< Energy deposited in dE detector.                   */
                          QVector<double> d_dE, /*!< Uncertainty in energy deposited in dE detector.    */
                          QVector<double> E,    /*!< Energy deposited in E detector.                    */
                          QVector<double> d_E,  /*!< Uncertainty in energy deposited in E detector.     */
                          Result_t &where       /*!< Structure to store the data.                       */)
    {
        where.Ex = Ex;
        where.dE = dE;
        where.d_dE = d_dE;
        where.E = E;
        where.d_E = d_E;
        where.is_set = true;
    }

    //! Function to make a table from a result structure.
    /*! \return the html code for the table.
     */
    QString makeTable(Result_t what /*!< Strucure containing the data to write to table.    */);

    //! Fuction to make a table from a result structure.
    /*! \return the txt string used for the table.
     */
    QString makeTableTXT(Result_t what /*!< Structure containing the data to write to table.    */);

    //! Function to make html code from the coefficients.
    /*! \return the html code for nice output of the coefficients.
     */
    QString makeCoeff(QVector<double> coeff /*!< Vector containing the coefficients.    */);

    //! Function to make txt code from the coefficients.
    /*! \return the txt string for nice output of the coefficients.
     */
    QString makeCoeffTXT(QVector<double> coeff /*!< Vector containing the coefficients. */);
};

#endif // TABLEMAKERHTML_H
