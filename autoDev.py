#!/usr/bin/env python

from subprocess import call, Popen
import configparser
import os
import xml.etree.ElementTree
import itertools

## Global settings
execFile = "./scenarios/debug/devScenario" ;
globalConfFile = "./config-files/DEV/GLOB_CONF.cfg";
userConfFile = "./config-files/DEV/USER.xml";

Popen("rm " + os.getcwd() + '/tests/*.dat', shell=True)
## Function to call the executable
def callScenario (pcmode, asmode) : 
    call([execFile, pcmode, asmode])

## Change parameter of global configuration file    
def changeGlobalParam (params):
    if not os.path.exists(globalConfFile):
        print("Global configuration file does not exist");
        exit();
    
    config = configparser.SafeConfigParser();
    config.optionxform = str;
    config.read(globalConfFile);
    config.set(*params)
    with open(globalConfFile, "w") as config_file:
        config.write(config_file, space_around_delimiters=False)

## Change xml parameter
def ChangeConfParam (fn, l1, l2, val, l3 = None) :
    et = xml.etree.ElementTree.parse(fn);
    root = et.getroot();
    if (l3 != None) : 
        root.find(l1).find(l2).find(l3).text = val;
    else :
        root.find(l1).find(l2).text = val;
    et.write(fn)
    
        
## Configure the scenario
nUsers = [20, 40, 60];#range(50,200+1,100);
alphas = [1];
sinrs = [5];#range(0,15+1,15);
npicos = [0, 10];#range(10,100+1,20);
nbias = [3, 6, 9];
for users, sinr, picos, alpha in itertools.product(nUsers, sinrs, npicos, alphas):
    ChangeConfParam (userConfFile, "CRE", "BIAS_PICO", "0");
    changeGlobalParam( ("PICO", "NUMBER", str(picos) ));
    changeGlobalParam( ("USER", "NUMBER", str(users) ));
    ChangeConfParam (userConfFile, "LTE_UE", "UL_SINR_TH", str(sinr));
    ChangeConfParam (userConfFile, "LTE_UE", "UL_ALPHA", str(alpha));
    print("OL - Users %02d, SINR_th %02d, picos %03d, alpha %f" %  (users, sinr, picos, alpha) );
    callScenario ("ol", "rsrp");
    print("    RSRP")
    callScenario ("ol", "pl");
    print("    PL")
    for picoBias in nbias:
        ChangeConfParam (userConfFile, "CRE", "BIAS_PICO", str(picoBias));
        callScenario ("ol", "cre");
        print("    CRE %s" % picoBias)

alphas = [1.0];
for users, sinr, picos, alpha in itertools.product(nUsers, sinrs, npicos, alphas):
    ChangeConfParam (userConfFile, "CRE", "BIAS_PICO", "0");
    changeGlobalParam( ("PICO", "NUMBER", str(picos) ));
    changeGlobalParam( ("USER", "NUMBER", str(users) ));
    ChangeConfParam (userConfFile, "LTE_UE", "UL_SINR_TH", str(sinr));
    ChangeConfParam (userConfFile, "LTE_UE", "UL_ALPHA", str(alpha));
    print("LP - Users %02d, SINR_th %02d, picos %03d, alpha %f" %  (users, sinr, picos, alpha) );
    callScenario ("lp", "rsrp");
    print("    RSRP")
    callScenario ("lp", "pl");
    print("    PL")
    for picoBias in nbias:
        ChangeConfParam (userConfFile, "CRE", "BIAS_PICO", str(picoBias));
        callScenario ("lp", "cre");        
        print("    CRE %s" % picoBias)