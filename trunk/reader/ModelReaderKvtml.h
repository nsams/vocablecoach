#ifndef MODELREADERKVTML_H
#define MODELREADERKVTML_H
#include <QString>
#include "ModelReaderAbstract.h"

class VocableListModel;

class ModelReaderKvtml : public ModelReaderAbstract
{
public:
    ModelReaderKvtml(const QString& filename);
    ~ModelReaderKvtml() { }
    bool read(VocableListModel* model, QUndoStack* undoStack);
    bool isValidFile();
};

#endif
