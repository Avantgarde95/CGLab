#ifndef APP_EXTERNAL_MODEL_HPP
#define APP_EXTERNAL_MODEL_HPP

#include "App.hpp"

namespace App {
    class ExternalModel : public Engine::OBJModel<GeneralModel> {
    public:
        explicit ExternalModel(const std::string &path);
    };
}

#endif
