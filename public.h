#ifndef PUBLIC_H
#define PUBLIC_H

#include <graphics.h>

typedef enum {
    PAGE_HOME,
    PAGE_LOGIN,
    PAGE_PERSONAL_MANAGEMENT,
    PAGE_PERSONAL_REGISTRATION,
    PAGE_PERSONAL_INSPECTION
} PageType;

typedef enum {
    MOUSE_NONE,
    HOME_BTN_PERSON,
    HOME_BTN_SHARE,
    LOGIN_BACK,
    LOGIN_INPUT_USER,
    LOGIN_INPUT_PHONE,
    LOGIN_INPUT_CODE,
    LOGIN_SUBMIT,
    PERSON_BTN_BACK,
    PERSON_BTN_REG,
    PERSON_BTN_YEAR,
    PERSON_BTN_CHANGE,
    PERSON_BTN_REPAIR,
    PERSON_BTN_ENTRY_EXIT
} MouseTarget;

extern PageType currentPage;
extern MouseTarget currentMouseCtrl;

#endif
