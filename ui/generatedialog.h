#ifndef GENERATE_DIALOG_H
#define GENERATE_DIALOG_H

#include "modeltemplate.h"

namespace CP {
    namespace UI {
        class GenerateDialog : public ModelTemplate {
            Q_OBJECT

        public:
            explicit GenerateDialog(QWidget *parent = nullptr);
            QStringList getData() override;
        };
    } // namespace UI;
} // namespace CP;

#endif
