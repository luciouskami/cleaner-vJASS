#include "keywords.hpp"

const char* Keywords::globals = "globals";
const char* Keywords::udg_ = "udg_*";
const char* Keywords::gg_ = "gg_*";
const char* Keywords::endglobals = "endglobals";
const char* Keywords::InitGlobals = "function InitGlobals takes nothing returns nothing";
const char* Keywords::Trig_ = "function Trig_*";
const char* Keywords::InitTrig_ = "function InitTrig_*";
const char* Keywords::InitCustomTriggers = "function InitCustomTriggers takes nothing returns nothing";
const char* Keywords::RunInitializationTriggers = "function RunInitializationTriggers takes nothing returns nothing";
const char* Keywords::InitCustomPlayerSlots = "function InitCustomPlayerSlots takes nothing returns nothing";
const char* Keywords::InitCustomTeams = "function InitCustomTeams takes nothing returns nothing";
const char* Keywords::main = "function main takes nothing returns nothing";
const char* Keywords::ExecuteFunc = "call ExecuteFunc*";
const char* Keywords::config = "function config takes nothing returns nothing";
const char* Keywords::endfunction = "endfunction";
const char* Keywords::import = "// BEGIN IMPORT OF *";
const char* Keywords::import2 = "//! import \"*";