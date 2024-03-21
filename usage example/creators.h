#ifndef CREATORS_H
#define CREATORS_H

#include <utility>

template<typename Base>
class AbstractCreator {
public:

    virtual Base* create() const = 0;

    virtual ~AbstractCreator() = default;
};

template<typename Base, typename Derived>
class WorldObjectCreator : public AbstractCreator<Base> {
public:

    Base* create() const override {
        return new Derived();
    }

    virtual ~WorldObjectCreator() override = default;
};

#endif // CREATORS_H
