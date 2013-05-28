/*!
 * \file main.c
 * \brief Краткое описание
 *
 * Полное описание
 *
 * $Id: $
 *
 * \author Сергей Хватов <xbatob@grouptechno.ru>, (C) 2009
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "gettext.h"

#ifndef LOCALEDIR
#define LOCALEDIR NULL
#endif

#ifndef PACKAGE_STRING
#define PACKAGE_STRING "template 0.0"
#define PACKAGE_BUGREPORT "Sergey Khvatov <xbatob@grouptechno.ru>"
#endif
#define MY_VERSION     PACKAGE_STRING \
    "\n$Id: $"

#include <locale.h>
#include <argp.h>
#include <stdarg.h>

#include <iostream>
#include <fstream>

#include <libconfig.h++>

#include <json/value.h>
#include <json/writer.h>

using namespace std;
using namespace libconfig;

const char *argp_program_version = MY_VERSION;
const char *argp_program_bug_address = PACKAGE_BUGREPORT;

void parse_setting (Setting const &set, Json::Value &dest) {
    Setting::Type t = set.getType ();
    switch (t) {
    case Setting::TypeGroup:
        for (int i = 0; i < set.getLength(); ++i) {
            Setting const &s = set[i];
            parse_setting (s, dest[s.getName()]);
        }
        break;
    case Setting::TypeArray:
    case Setting::TypeList:
        for (int i = 0; i < set.getLength(); ++i) {
            Setting const &s = set[i];
            parse_setting (s, dest[i]);
        }
        break;
    case Setting::TypeBoolean:
        dest = (bool) set;
        break;
    case Setting::TypeInt:
        dest = (int) set;
        break;
    case Setting::TypeInt64:
        dest = (long long int) set;
        break;
    case Setting::TypeString:
        dest = set.c_str();
        break;
    case Setting::TypeFloat:
        dest = (double) set;
        break;
    default:
        cerr << "setting ignored: " << set.getPath() << endl;
    }
}

void config2json (const char *filename) {
    Config cf;
    try {
        cf.readFile (filename);
    } catch (ConfigException const &e) {
        cerr << filename << ": load error: " << e.what() << endl;
        return;
    }

    try {
        Json::Value root;
        parse_setting (cf.getRoot(), root);
        cout << root;
    } catch (SettingException const &e) {
        cerr << e.what() << " on " <<  e.getPath() << endl;
    }
}

static error_t apf (int key, char *arg, struct argp_state *state) {
    switch (key) {
    case 'o': {
        filebuf *fb = new filebuf;
        if (!fb->open (arg, ios_base::out | ios_base::trunc)) {
            argp_failure (state, 1, errno, "Can not open %s", arg);
        }
        cout.rdbuf (fb);
        break;
    }
    case ARGP_KEY_ARG:
        config2json (arg);
        break;
    default:
        return  ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static const struct argp_option apo[] = {
    {
        "output", 'o', "FILE", 0,
        N_ ("Output to FILE")
    }, {
        0
    }
};

static const struct argp ap = {
    apo, apf, N_ ("config file..."),
    N_ ("convert configuration from libconfig format"
    " to JSON one"),
    0, 0
};

int main (int argc, char *argv[]) {
#if         ENABLE_NLS
    setlocale (LC_ALL, "");
    bindtextdomain (PACKAGE, LOCALEDIR);
    textdomain (PACKAGE);
#endif  //  ENABLE_NLS

    argp_parse (&ap, argc, argv, 0, 0, 0);
    return 0;
}
