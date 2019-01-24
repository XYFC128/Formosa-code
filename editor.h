#ifndef EDITOR_H
#define EDITOR_H
#include <Qsci/qsciscintilla.h>
class QsciScintilla;

class CodeEditor : public QWidget{
public:
    CodeEditor();
    ~CodeEditor();
private:
    QsciScintilla *textEdit;
};
#endif // EDITOR_H
