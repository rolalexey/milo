#include "dict.h"
#include <QFile>
#include <QDebug>

/*!
 * \class AnagramFinder
 */
AnagramFinder::AnagramFinder(QObject * parent) :
    QThread(parent),
    m_wcount(0)
{
    setObjectName("Dict");
}

AnagramFinder::~AnagramFinder()
{
    if(isRunning())
        terminate();
}

void AnagramFinder::resetDict()
{
    foreach (WHashPtr wh, m_wordPools) {
        wh->clear();
    }
    m_wcount = 0;
}

void AnagramFinder::resetResult()
{
    m_longest.clear();
    m_work.clear();
}

void AnagramFinder::loadFile(const QString &fname)
{
    QFile   dictFile(fname);
    if(dictFile.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        resetDict();
        while(!dictFile.atEnd())
        {
            QString word = dictFile.readLine();
            word.remove('\n');

            addWord( word );
        }
    }
}

int AnagramFinder::wordCount()
{
    return m_wcount;
}


/*!
 * \brief AnagramFinder::searchAnagram
 * return
 * \fn QStringList searchAnagram(const QString &word)
 */
QStringList AnagramFinder::searchAnagram(const QString &word)
{
    QStringList anlist;
    int len = word.length();

    if( m_wordPools.contains(len) )
    {
        auto w = m_wordPools[len];
        foreach (auto dword, w->keys()) {
            if( checkAnagram(word, dword) )
                anlist << dword;
        }
    }

    return anlist;
}


/*!
 * \brief AnagramFinder::addWord
 * \fn void addWord(const QString &word)
 *
 *
 */
void AnagramFinder::addWord(const QString &word)
{
    QString buf = word.toLower();
    int len = word.length();
    if( !m_wordPools.contains(len) )
    {
        m_wordPools[len] = WHashPtr( new wHash );
    }

    /*!
     * \brief get hash pointer corresponding word length
     */
    WHashPtr w = m_wordPools[len];

    /*!
        m_wordPools  contains pointers to hashes each one containing words of same size.
                     Every word length correnponds individual hash.
    */

    if(buf.contains(' '))       return;
    if(buf.contains('-'))       return;
    if( w->contains(buf) )      return;

    w->insert(buf, 1);
    m_wcount++;

}


/*!
 * \brief AnagramFinder::checkAnagram autually check if two words forms anagram
 * \fn bool checkAnagram(const QString &word, const QString &dword)
 */
bool AnagramFinder::checkAnagram(const QString &word, const QString &dword)
{

    // Word cannot be anagram of self
    if(word == dword)   return false;


    // 127 means we check anagram only fro english alphabet (first half of ASCII table)

    // fill  codes[] woth zeros
    int codes[127] = {};

    int min=127,max=0;

    // 1. add 1 to character position in codes[] on each occurance in first word
    foreach (QChar ch, word) {
        int n = ch.toLatin1();
        codes[ n ] ++;
        if(n < min) min = n;
        if(n > max) max = n;
    }
    // 1. substract 1 from character position in codes[] on each occurance in second word
    foreach (QChar ch, dword) {
        int n = ch.toLatin1();
        codes[ n ] --;
        if(n < min) min = n;
        if(n > max) max = n;
    }

    // in case of anagram all positions in codes[] shall return to zeros.
    for (int i=min; i<max; i++) {
        if(codes[i] != 0)   return false;
    }



    return true;
}


/*!
 * \brief AnagramFinder::searchLongest search longest anagrans chain
 * return string, containing letters of anagram chain, if any
 * \fn QString searchLongest(const QString &word, int level)
 *
 * searchLongest use recursive algorythm, adding sequentially letters from 'a' to 'z' to starting text and
 * checking if anagrams exists. If so, it call itself using starting word with added letter and recursion
 * level increased.
 * \sa checkAnagram
 */
QString AnagramFinder::searchLongest(const QString &word, int level)
{



    // Progress emitted only in first level of recursion
    if(level == 0)
        emit progress(0);

    for (int ch = 'a'; ch <= 'z'; ch++)
    {

       QChar c(ch);

       auto l_next = word + c;


       QStringList anList = searchAnagram(l_next);

       m_work.push_front(c);
       if(anList.count())
       {
           if(m_work.count() > m_longest.count())
                m_longest = m_work;
           searchLongest(l_next, level+1);

       }
       if(!m_work.isEmpty())
            m_work.pop_front();


    int percent = (ch-'a')*100/('z'-'a');
    // Progress emitted only in first level of recursion
       if(level == 0)
           emit progress(percent);
    }


    QString res;
    for(auto it = m_longest.rbegin(); it != m_longest.rend(); it++)
        res += *it;

    return res;
}


/*!
 * \brief AnagramFinder::startSearchLongest public slot starting anagram search in another thread.
 * \fn void startSearchLongest(const QString &word)
 * \sa searchLongest
 */
void AnagramFinder::startSearchLongest(const QString &word)
{
    m_startw = word;
    start(QThread::HighPriority);
}

/*!
 * \brief AnagramFinder::run actual worker
 * \fn void run()
 */
void AnagramFinder::run()
{
    auto longest = searchLongest(m_startw);
    emit longestChain( longest );
}


