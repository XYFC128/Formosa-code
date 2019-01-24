#include "editor.h"
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qsciapis.h>
CodeEditor::CodeEditor(){
}
CodeEditor::~CodeEditor(){
    delete textEdit;
}
