#ifndef GRAPHICS_PIPELINE_H
#define GRAPHICS_PIPELINE_H

#include <map>
#include <memory>

#include "creators.h"

template<typename Base, typename ObjType>
class GraphicsPipeline {
protected:

    typedef AbstractCreator<Base> AbstractFactory;

    typedef std::map<ObjType, AbstractFactory*> FactoryMap;

public:

    Base* pullout(ObjType id) const {

        typename FactoryMap::const_iterator it = graphic_objects.find(id);

        if (it != graphic_objects.end())
            return it->second->create();

        return nullptr;
    }

    template<typename SceneObject>
    void add(ObjType id) {
        register_type(id, new WorldObjectCreator<Base, SceneObject>);
    }

    void remove(ObjType id) {

        typename FactoryMap::const_iterator it = graphic_objects.find(id);

        if (it == graphic_objects.end())
            return;

        delete it->second;
        graphic_objects.erase(it);
    }

    ~GraphicsPipeline() {
        for (typename FactoryMap::iterator it = graphic_objects.begin(); it != graphic_objects.end(); ++it)
            delete it->second;
    }

    bool is_registred(ObjType id) const {
        return graphic_objects.find(id) != graphic_objects.end();
    }

    std::size_t size() const {
        return graphic_objects.size();
    }

protected:

    void register_type (ObjType id, AbstractFactory* pAbstractFactory) {

        std::unique_ptr<AbstractFactory> ptr(pAbstractFactory);

        typename FactoryMap::const_iterator it = graphic_objects.find(id);

        if (it == graphic_objects.end())
            graphic_objects[id] = ptr.release();
    }

private:
    FactoryMap graphic_objects;
};

#endif // GRAPHICS_PIPELINE_H
