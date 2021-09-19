#include <pybind11/embed.h> // everything needed for embedding
#include <pybind11/eval.h>

#include "Paths.h"

#include <string>


namespace py = pybind11;
using namespace pybind11::literals;


class Python
{
public:
    py::module_ xtb;

    Python(std::string scriptsDir);
    ~Python();

    bool checkImportedModules(std::string importedModuleName);

    void calcDistributions(Paths& paths);

    void getDistributions(Paths& paths);

};


Python::Python(std::string scriptsDir)
{

    //py::scoped_interpreter guard{};

    py::module_ sys = py::module_::import("sys");
    sys.attr("path").attr("append")(scriptsDir); // append REPM//share//scripts//.

    xtb = py::module_::import("xtb_c_plus_plus_bindings");
};

Python::~Python() {
    //std::cout << "==================================================" << std::endl;
}

bool Python::checkImportedModules(std::string importedModuleName) {

    py::module_ sys = py::module_::import("sys");
    py::list modules = sys.attr("modules");

    for (auto item : modules)
        if (item.cast<std::string>() == importedModuleName) return true;

    return false;
}

void Python::calcDistributions(Paths& paths) {
    py::dict start_kwargs = py::dict(
        "mat"_a = paths.mat.toStdString(),
        "par"_a = paths.par.toStdString(),
        "home"_a = paths.home.toStdString(),
        "proj"_a = paths.proj.toStdString(),
        "rmp"_a = paths.proj.toStdString() + "/asd",
        "tab"_a = paths.tab.toStdString(),
        "lay"_a = paths.lay.toStdString(),
        "photon"_a = paths.photon,
        "electron"_a = paths.electron);


    xtb.attr("calc_distributions")(**start_kwargs);
}

void Python::getDistributions(Paths& paths) {
    py::dict start_kwargs = py::dict(
        "mat"_a = paths.mat.toStdString(),
        "par"_a = paths.par.toStdString(),
        "home"_a = paths.home.toStdString(),
        "proj"_a = paths.proj.toStdString(),
        "rmp"_a = paths.proj.toStdString() + "/asd",
        "tab"_a = paths.tab.toStdString(),
        "lay"_a = paths.lay.toStdString(),
        "photon"_a = paths.photon,
        "electron"_a = paths.electron);


    xtb.attr("get_distributions")(**start_kwargs);
}


namespace PythonBinds {

    Python* python;

    void start_interpreteter() {
        py::initialize_interpreter();
    }

    void initiateInterpritater(std::string scriptsPath) {
        python = new Python(scriptsPath);
    }

    void finalize_interpreteter() {
        //delete& python;
        py::finalize_interpreter();
    }

    int calcDistribution(Paths& paths) {

        try
        {
            python->calcDistributions(paths);
        }
        catch (const std::exception&)
        {
            return 1;
        }

        return 0;
    }


    int getDistribution(Paths& paths) {

        try
        {
            python->getDistributions(paths);
        }
        catch (const std::exception&)
        {
            return 1;
        }
        return 0;
    }

}



