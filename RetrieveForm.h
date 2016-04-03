/* 
 * File:   RetrieveForm.h
 * Author: bx830
 *
 * Created on 28 August 2015, 6:12 PM
 */

#ifndef _RETRIEVEFORM_H
#define	_RETRIEVEFORM_H
#include <string>

#include "ui_RetrieveForm.h"

class RetrieveForm : public QDialog {
    Q_OBJECT
public:
    const static int CANCEL = 0;
    const static int OK = 1;
    RetrieveForm();
    virtual ~RetrieveForm();
    std::string getFileName();
public slots:
       void SelectFile();
       void ok();
       void cancel();
    
private:
    Ui::RetrieveForm widget;
    std::string fileName;
};

#endif	/* _RETRIEVEFORM_H */
