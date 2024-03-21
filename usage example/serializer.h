#pragma once

#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>

#include <dimensional.h>
#include <skybox.h>

#define OBJ_PATH "obj_file"
#define ROTATION "rotation"
#define SCALE "scale"
#define LOCATION "location"
#define TEXTURE "texture"
#define TYPE "object"
#define SKYBOX_WIDTH "width"

namespace serializer {

QJsonObject decompose_vector(const QVector3D &vec) {

    QJsonObject vec_json;

    vec_json.insert("x", vec.x());
    vec_json.insert("y", vec.y());
    vec_json.insert("z", vec.z());

    return vec_json;
}

QVector3D build_vector(QJsonObject vec_json) {

    float x = vec_json.value("x").toDouble();
    float y = vec_json.value("y").toDouble();
    float z = vec_json.value("z").toDouble();

    return QVector3D(x, y, z);
}


QJsonObject decompose_quaternion(const QQuaternion& q) {

    QJsonObject q_json;

    q_json.insert("x", q.x());
    q_json.insert("y", q.y());
    q_json.insert("z", q.z());
    q_json.insert("scalar", q.scalar());

    return q_json;
}


QQuaternion build_quaternion(QJsonObject q_json) {

    float x = q_json.value("x").toDouble();
    float y = q_json.value("y").toDouble();
    float z = q_json.value("z").toDouble();
    float s = q_json.value("scalar").toDouble();

    return QQuaternion(s, x, y, z);
}


skybox* unmarshalize_skybox(const QJsonObject& json_obj) {

    skybox* s = new skybox(json_obj.value(SKYBOX_WIDTH).toDouble(), json_obj.value(TEXTURE).toString());
    s->translate(build_vector(json_obj.value(LOCATION).toObject()));

    return s;
}

dimensional* unmarshalize_dimensional(const QJsonObject& json_obj) {

        dimensional* d = new dimensional();

        d->loadObjectFromFile(json_obj.value(OBJ_PATH).toString());
        d->scale(json_obj.value(SCALE).toDouble());
        d->translate(build_vector(json_obj.value(LOCATION).toObject()));
        d->rotate(build_quaternion(json_obj.value(ROTATION).toObject()));

        return d;
    }


    auto marshalize_skybox(const skybox* s) {

        QJsonObject obj;

        obj.insert(TYPE, "skybox");
        obj.insert(TEXTURE, s->GetTexture());
        obj.insert(LOCATION, decompose_vector(s->getLocation()));
        obj.insert(SKYBOX_WIDTH, s->width());
        return obj;
    }

    auto marshalize_dimensional(const dimensional* d) -> QJsonObject {

        QJsonObject obj;

        obj.insert(TYPE, "3d model");
        obj.insert(OBJ_PATH, d->GetFilePath());
        obj.insert(SCALE, d->GetScale());
        obj.insert(LOCATION, decompose_vector(d->GetLocation()));
        obj.insert(ROTATION, decompose_quaternion(d->GetRotation()));

        return obj;
    }

    void marshalize(const QVector<dimensional*> &scene_objects, const QVector<skybox*> &skyboxes, const QString& dest_file) {

        QJsonArray json_data;

        for(qsizetype i = 0; i < scene_objects.size(); ++i)
            json_data.append(marshalize_dimensional(scene_objects[i]));

        for(qsizetype i = 0; i < skyboxes.size(); ++i)
            json_data.append(marshalize_skybox(skyboxes[i]));

        QJsonDocument doc(json_data);

        QFile file(dest_file);

        if(!file.open(QIODevice::WriteOnly))
            throw;

        file.write(doc.toJson());
        file.close();
    }

    void unmarshalize(QVector<dimensional*> &dest_objects, QVector<skybox*> &dest_skyboxes, const QString &dest_file) {

        QFile file(dest_file);
        file.open(QIODevice::ReadOnly);
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();

        auto json_data = doc.array();

        for(auto json_val: json_data) {

            auto json_obj = json_val.toObject();

            if (json_obj.value(TYPE).toString() == "3d model")
                dest_objects.append(unmarshalize_dimensional(json_obj));
            else
                dest_skyboxes.append(unmarshalize_skybox(json_obj));
        }
    }
}
#endif // SERIALIZER_H
