#include "CueSheetParser.h"
#include <util/PluginHelper.h>

static const PluginInfo info = {
    "Yanhui Shen",
    "Cue Pack",
    "Cue sheet parser.",
    2
};

MOUS_DEF_PLUGIN(PluginType::SheetParser, &info, CueSheetParser);
