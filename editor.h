#ifndef EDITOR_H
#define EDITOR_H
#include <Qsci/qsciscintilla.h>
class codeEditor : public QsciScintilla{
public:
    bool loadFile(const QString &fileName);
    bool maybeSave();
    bool save();
    bool saveAs(QString);
         codeEditor();
    void newFile();
         ~codeEditor();

    QString curFile;
    QString title;

private:
    QString strippedName(const QString &fullFileName);
    bool    SetupEditor();
    bool    SetupKeyList();
    void    setCurrentFile(const QString &fileName);

    QsciLexer*textLexer;
    QStringList cppkeylist,
    pykeylist;
};
#endif // EDITOR_H
