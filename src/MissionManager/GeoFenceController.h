/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#ifndef GeoFenceController_H
#define GeoFenceController_H

#include "PlanElementController.h"
#include "GeoFenceManager.h"
#include "QGCMapPolygon.h"
#include "Vehicle.h"
#include "MultiVehicleManager.h"
#include "QGCLoggingCategory.h"

Q_DECLARE_LOGGING_CATEGORY(GeoFenceControllerLog)

class GeoFenceManager;

class GeoFenceController : public PlanElementController
{
    Q_OBJECT
    
public:
    GeoFenceController(QObject* parent = NULL);
    ~GeoFenceController();

    Q_PROPERTY(bool                 circleEnabled       READ circleEnabled                                      NOTIFY circleEnabledChanged)
    Q_PROPERTY(float                circleRadius        READ circleRadius                                       NOTIFY circleRadiusChanged)

    Q_PROPERTY(bool                 polygonEnabled      READ polygonEnabled                                     NOTIFY polygonEnabledChanged)
    Q_PROPERTY(QGCMapPolygon*       mapPolygon          READ mapPolygon                                         CONSTANT)

    Q_PROPERTY(bool                 breachReturnEnabled READ breachReturnEnabled                                NOTIFY breachReturnEnabledChanged)
    Q_PROPERTY(QGeoCoordinate       breachReturnPoint   READ breachReturnPoint      WRITE setBreachReturnPoint  NOTIFY breachReturnPointChanged)

    Q_PROPERTY(QVariantList         params              READ params                                             NOTIFY paramsChanged)
    Q_PROPERTY(QStringList          paramLabels         READ paramLabels                                        NOTIFY paramLabelsChanged)
    Q_PROPERTY(QString              editorQml           READ editorQml                                          NOTIFY editorQmlChanged)

    Q_INVOKABLE void addFence(void);
    Q_INVOKABLE void removeFence(void);

    void start                      (bool editMode) final;
    void startStaticActiveVehicle   (Vehicle* vehicle) final;
    void loadFromVehicle            (void) final;
    void sendToVehicle              (void) final;
    void loadFromFile               (const QString& filename) final;
    void saveToFile                 (const QString& filename) final;
    void removeAll                  (void) final;
    void removeAllFromVehicle       (void) final;
    bool syncInProgress             (void) const final;
    bool dirty                      (void) const final;
    void setDirty                   (bool dirty) final;
    bool containsItems              (void) const final;

    QString fileExtension(void) const final;

    bool                circleEnabled       (void) const;
    bool                polygonEnabled      (void) const;
    bool                breachReturnEnabled (void) const;
    float               circleRadius        (void) const;
    QGCMapPolygon*      mapPolygon          (void) { return &_mapPolygon; }
    QGeoCoordinate      breachReturnPoint   (void) const { return _breachReturnPoint; }
    QVariantList        params              (void) const;
    QStringList         paramLabels         (void) const;
    QString             editorQml           (void) const;

    void setBreachReturnPoint(const QGeoCoordinate& breachReturnPoint);

signals:
    void addFencePolygon            (void);
    void circleEnabledChanged       (bool circleEnabled);
    void polygonEnabledChanged      (bool polygonEnabled);
    void breachReturnEnabledChanged (bool breachReturnEnabled);
    void circleRadiusChanged        (float circleRadius);
    void breachReturnPointChanged   (QGeoCoordinate breachReturnPoint);
    void paramsChanged              (QVariantList params);
    void paramLabelsChanged         (QStringList paramLabels);
    void editorQmlChanged           (QString editorQml);
    void loadComplete               (void);

private slots:
    void _polygonDirtyChanged(bool dirty);
    void _setDirty(void);
    void _setPolygonFromManager(const QList<QGeoCoordinate>& polygon);
    void _setReturnPointFromManager(QGeoCoordinate breachReturnPoint);
    void _loadComplete(const QGeoCoordinate& breachReturn, const QList<QGeoCoordinate>& polygon);
    void _updateContainsItems(void);

private:
    void _init(void);
    void _signalAll(void);
    bool _loadJsonFile(QJsonDocument& jsonDoc, QString& errorString);

    void _activeVehicleBeingRemoved(void) final;
    void _activeVehicleSet(void) final;

    bool                _dirty;
    QGCMapPolygon       _mapPolygon;
    QGeoCoordinate      _breachReturnPoint;
    QVariantList        _params;

    static const char* _jsonFileTypeValue;
    static const char* _jsonBreachReturnKey;
};

#endif
