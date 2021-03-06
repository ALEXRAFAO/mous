#include "LameEncoder.h"
#include <util/PluginHelper.h>

static const PluginInfo info = {
    "Yanhui Shen",
    "Lame Encoder",
    "Fast MP3 encoder.",
    1
};

MOUS_DEF_PLUGIN(PluginType::Encoder, &info, LameEncoder);
