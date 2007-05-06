#ifndef MODELREADERPAUKER_H
#define MODELREADERPAUKER_H
#include <QString>
#include "ModelReaderAbstract.h"

class VocableListModel;
class QUndoStack;

class ModelReaderPauker : public ModelReaderAbstract
{
public:
    ModelReaderPauker(const QString& filename);
    bool read(VocableListModel* model, QUndoStack* undoStack);
    bool isValidFile();
};

#endif
