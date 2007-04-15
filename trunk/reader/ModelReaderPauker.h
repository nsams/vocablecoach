#ifndef MODELREADERPAUKER_H
#define MODELREADERPAUKER_H
#include <QString>
#include "ModelReaderAbstract.h"

class VocableListModel;

class ModelReaderPauker : public ModelReaderAbstract
{
public:
    ModelReaderPauker(const QString& filename);
    bool read(VocableListModel* model);
    bool isValidFile();
};

#endif
