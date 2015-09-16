#set(hdrs Thread.h Timer.h TimeSpec.h libkobuki.h Kobuki.h)
set(hdrs stargazer.h)

set(install_hdrs StarGazer.h)

install(FILES ${install_hdrs} DESTINATION ${INC_INSTALL_DIR}
    COMPONENT library)

