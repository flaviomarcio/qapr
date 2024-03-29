#pragma once

#include "../../qstm/src/qstm_startup.h"
#include "./qapr_global.h"

namespace QApp {

#define Q_APP_STARTUP_FUNCTION(FUNC) Q_STM_STARTUP_APPEND(QApp, FUNC)

//!
//! \brief The StartUp class
//!
class Q_APR_EXPORT StartUp : public QStm::StartUp
{
    Q_OBJECT
    Q_STM_STARTUP_CONSTUCTOR(4)
};

} // namespace QRpc
