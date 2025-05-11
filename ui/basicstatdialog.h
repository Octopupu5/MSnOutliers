#ifndef STAT_DIALOG_H
#define STAT_DIALOG_H

#include "modeltemplate.h"

namespace CP {
    namespace UI {
        class BasicStatDialog : public ModelTemplate {
            Q_OBJECT

        public:
            explicit BasicStatDialog(QWidget *parent = nullptr);
            QStringList getData() override;
        };
    } // namespace UI;
} // namespace CP;

#endif // STAT_DIALOG_H
