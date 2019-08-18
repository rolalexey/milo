#include "mtranslator.h"
#include <QDebug>

MTranslator::MTranslator(QObject *parent) :
    QObject(parent)
{
    setObjectName("MTranslator");
    setMorseMode(true);
    initMorseMap();
}

MTranslator::~MTranslator()
{

}

bool MTranslator::morseMode()
{
    return m_morse;

}


/*!
 * \brief MTranslator::isMorseCoded
 * \param text
 * \return true if text is Morse encoded
 */
bool MTranslator::isMorseCoded(const QString &text)
{

    // Morse means that text contains only  '.', '-', ' ', '\n'
    foreach (const auto ch, text) {
        if( ch != '.' && ch != '-' && ch != ' ' && ch != '\n')
            return false;
    }

    return true;
}

void MTranslator::setSource(const QString &text)
{
    m_source = text;
    m_result.clear();
    setMorseMode( isMorseCoded(m_source) );

    emit sourceChanged(m_source);
}

QString &MTranslator::result()
{
    return m_result;
}

void MTranslator::translate()
{

    if(morseMode())
        fromMorse(m_source);
    else
        toMorse(m_source);

    emit    translated(m_result);
}

void MTranslator::setMorseMode(bool mode)
{
    m_morse = mode;
    emit morseMode(m_morse);
}

void MTranslator::initMorseMap()
{
    addMap("A", ".-");
    addMap("B", "-...");
    addMap("C", "-.-.");
    addMap("D", "-..");
    addMap("E", ".");
    addMap("F", "..-.");
    addMap("G", "--.");
    addMap("H", "....");
    addMap("I", "..");
    addMap("J", ".---");
    addMap("K", "-.-");
    addMap("L", ".-..");
    addMap("M", "--");
    addMap("N", "-.");
    addMap("O", "---");
    addMap("P", ".--.");
    addMap("Q", "--.-");
    addMap("R", ".-.");
    addMap("S", "...");
    addMap("T", "-");
    addMap("U", "..-");
    addMap("V", "...-");
    addMap("W", ".--");
    addMap("X", "-..-");
    addMap("Y", "-.--");
    addMap("Z", "--..");

    addMap(".", ".-.-.-");
    addMap("!", "-.-.--");
    addMap("-", "-....-");

}

void MTranslator::addMap(const QString &A, const QString &M)
{
    m_tomorse[A] = M;
    m_totext[M] = A;
}

const QString &MTranslator::toMorse(const QString &text)
{

    m_result.clear();

    foreach (auto src_ch, text) {
        auto trc = m_tomorse.value( src_ch.toUpper() );
        if(m_result.length())   m_result += " ";

        m_result += trc;
    }

    return m_result;
}

const QString &MTranslator::fromMorse(const QString &text)
{
    m_result.clear();

    //! "Letters" in Morse code has a variable length, so we get list of 'letters'
    auto letters = text.split(" ");

    foreach(auto l, letters)
    {
        if(l.isEmpty()) m_result += " ";
        m_result += m_totext.value(l);
    }
    return m_result;
}
