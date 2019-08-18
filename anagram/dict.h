#ifndef DICT_H
#define DICT_H

#include <QString>
#include <QStringList>
#include <QHash>
#include <QSharedPointer>
#include <QObject>
#include <QQueue>
#include <QThread>
/*!
 * \page anagram-finder.html
 * \title Longest Anagram chain finder
 *
 *
 *
 */


typedef     QHash<QString, int>     wHash;
typedef     QSharedPointer<wHash>   WHashPtr;


/*!
 * \class AnagramFinder
 * \brief The AnagramFinder class allow to find word anagrams and longest chain of anagrams.
 * \ingroup anagram
 *
 */
class AnagramFinder : public QThread
{
    Q_OBJECT
public:
    AnagramFinder(QObject * parent);
    virtual ~AnagramFinder();

    /*!
       \fn resetDict()
     * \brief resetDict clears current word list.
     */
    void        resetDict();

    /*!
       \fn resetResult()
     * \brief resetResult
     */
    void        resetResult();

    /*!
     * \fn loadFile
     * \brief loadFile load word list from file
     * \param fname name of file
     *
     * Text file must contain words one per line. Multi word lines and words with '-' sign are skipped.
     */
    void        loadFile(const QString &fname);

    /*!
     * \brief wordCount
     * \return number of loaded words
     */
    int         wordCount();


    /*!
     * \brief searchAnagram find anagram for given word
     * \param word
     * \return list of possible anagrams in QStringList
     */
    QStringList searchAnagram(const QString &word);

    /*!
     * \brief addWord   add word to dictionary. If word already exist, nothing happens
     * \fn addWord
     * \param word
     */
    void        addWord(const QString &word);

    /*!
     * \brief checkAnagram check if two words is an anagram pair
     * \param word
     * \param dword
     * \return true if anagram found
     */
    bool        checkAnagram(const QString & word, const QString & dword);


    /*!
     * \brief searchLongest
     * \param word
     * \param level
     * \return
     */
    QString     searchLongest(const QString &word, int level = 0);

    /*!
     * \brief startSearchLongest start searching of longest anagram chain in current dictionary.
     * \param word starting text fragment
     *
     *
     */
    void        startSearchLongest(const QString & word);

protected:

    void        run() ;

signals:


    void        progress(int);
    void        longestChain(const QString &);

private:

     QHash<int, WHashPtr >     m_wordPools;
     int                       m_wcount;

     QString                   m_startw;

     QQueue<QChar>             m_work;
     QQueue<QChar>             m_longest;
};

#endif // DICT_H
