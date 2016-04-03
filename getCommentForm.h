/* 
 * File:   getCommentForm.h
 * Author: bx830
 *
 * Created on 28 August 2015, 5:48 PM
 */

#ifndef _GETCOMMENTFORM_H
#define	_GETCOMMENTFORM_H

#include "ui_getCommentForm.h"
#include <string>
using namespace std;

class getCommentForm : public QDialog {
    Q_OBJECT
public:
    const static int CANCEL = 0;
    const static int OK = 1;
    getCommentForm();
    virtual ~getCommentForm();
    string getComment();
public slots:
       void saveComment();
       void cancel();
private:
    Ui::getCommentForm widget;
    string commentText;
};

#endif	/* _GETCOMMENTFORM_H */
