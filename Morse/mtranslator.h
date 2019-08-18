#ifndef MTRANSLATOR_H
#define MTRANSLATOR_H
#include <QObject>
#include <QMap>


/*!
 * \class MTranslator
 * \brief The MTranslator class provides a Morse code translator.
 *
 * MTranslator encode source text to Morse and back to text. Direction of translation
 * is guessed automatically upon source text.
 */
class MTranslator : public QObject
{
    Q_OBJECT
public:
    MTranslator(QObject *parent = Q_NULLPTR);
    virtual ~MTranslator();

    /*!
     * \brief morseMode prepresent current state of translator.
     * \return true if source text is in Morse encoded
     */
    bool            morseMode();

    /*!
     * \brief isMorseCoded check if string in Mode code
     * \param text - string to check
     * \return true on More encoded string
     */
    bool            isMorseCoded(const QString & text);

    /*!
     * \brief setSource set new source text for translation.
     * \param text - new text to translate. text is copied to translator internal buffer.
     */
    void            setSource(const QString &text);

    /*!
     * \brief result - returns translated text.
     * \return reference to translated text.
     */
    QString &       result();

public slots:

    /*!
     * \brief translate - method starts translation.
     */
    void            translate();



protected:

    /*!
     * \brief setMorseMode set translation mode
     * \param mode
     */
    void            setMorseMode(bool mode);

    /*!
     * \brief initMorseMap - load Morse alphabet mapping to translator
     */
    void            initMorseMap();

    /*!
     * \brief addMap add single letter mapping to transltor
     * \param A letter in text
     * \param B lettet in Morse code
     */
    void            addMap(const QString & A, const QString & B);


    /*!
     * \brief toMorse translate from text to Morse
     * \param text
     * \return translated text
     */
    const QString & toMorse(const QString & text);

    /*!
     * \brief fromMorse translate from Morse to Text
     * \param text Morse encoded
     * \return decoded text
     */
    const QString & fromMorse(const QString & text);

signals:

    /*!
     * \brief morseMode signal emitted when translation mode changes
     * \param mode new mode
     */
    void            morseMode(bool mode);

    /*!
     * \brief sourceChanged signal emitted on source text changes
     */
    void            sourceChanged(const QString &);

    /*!
     * \brief translated emitted on translation finish
     */
    void            translated(const QString &);

private:
    QMap<QString, QString>  m_tomorse;
    QMap<QString, QString>  m_totext;

    bool    m_morse;

    QString m_source;
    QString m_result;

};

#endif // MTRANSLATOR_H
