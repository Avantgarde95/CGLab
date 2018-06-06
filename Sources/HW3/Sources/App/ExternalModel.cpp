#include "App.hpp"

namespace App {
    ExternalModel::ExternalModel(const std::string &path) : Engine::OBJModel<GeneralModel>(path) {}
}
