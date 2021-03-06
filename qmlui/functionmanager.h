/*
  Q Light Controller Plus
  functionmanager.h

  Copyright (c) Massimo Callegari

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0.txt

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

#ifndef FUNCTIONMANAGER_H
#define FUNCTIONMANAGER_H

#include <QStringList>
#include <QQuickView>
#include <QQuickItem>
#include <QObject>

#include "scenevalue.h"
#include "treemodel.h"

class Doc;
class Function;
class SceneEditor;
class FunctionEditor;
class GenericDMXSource;

typedef struct
{
    quint32 m_fID;
    QQuickItem *m_item;
} selectedFunction;

class FunctionManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariant functionsList READ functionsList NOTIFY functionsListChanged)
    Q_PROPERTY(int functionsFilter READ functionsFilter CONSTANT)
    Q_PROPERTY(QString searchFilter READ searchFilter WRITE setSearchFilter NOTIFY searchFilterChanged)
    Q_PROPERTY(int selectionCount READ selectionCount NOTIFY selectionCountChanged)
    Q_PROPERTY(bool isEditing READ isEditing NOTIFY isEditingChanged)
    Q_PROPERTY(int viewPosition READ viewPosition WRITE setViewPosition NOTIFY viewPositionChanged)

    Q_PROPERTY(int sceneCount READ sceneCount NOTIFY sceneCountChanged)
    Q_PROPERTY(int chaserCount READ chaserCount NOTIFY chaserCountChanged)
    Q_PROPERTY(int sequenceCount READ sequenceCount NOTIFY sequenceCountChanged)
    Q_PROPERTY(int efxCount READ efxCount NOTIFY efxCountChanged)
    Q_PROPERTY(int collectionCount READ collectionCount NOTIFY collectionCountChanged)
    Q_PROPERTY(int rgbMatrixCount READ rgbMatrixCount NOTIFY rgbMatrixCountChanged)
    Q_PROPERTY(int scriptCount READ scriptCount NOTIFY scriptCountChanged)
    Q_PROPERTY(int showCount READ showCount NOTIFY showCountChanged)
    Q_PROPERTY(int audioCount READ audioCount NOTIFY audioCountChanged)
    Q_PROPERTY(int videoCount READ videoCount NOTIFY videoCountChanged)

    Q_PROPERTY(QStringList audioExtensions READ audioExtensions CONSTANT)
    Q_PROPERTY(QStringList pictureExtensions READ pictureExtensions CONSTANT)
    Q_PROPERTY(QStringList videoExtensions READ videoExtensions CONSTANT)

public:
    FunctionManager(QQuickView *view, Doc *doc, QObject *parent = 0);
    ~FunctionManager();

    /*********************************************************************
     * Functions
     *********************************************************************/
    /** Read only property to expose the function tree to the QML UI */
    QVariant functionsList();

    /** Get a list of Functions that use $fid */
    Q_INVOKABLE QVariantList usageList(quint32 fid);

    /** Get a list of the currently selected Function IDs, suitable to be used in QML */
    Q_INVOKABLE QVariantList selectedFunctionsID();

    /** Get a list of the currently selected Function names, suitable to be used in QML */
    Q_INVOKABLE QStringList selectedFunctionsName();

    /** Enable/disable the display of a Function type in the functions tree */
    Q_INVOKABLE void setFunctionFilter(quint32 filter, bool enable);
    int functionsFilter() const;

    /** Get/Set a string to filter Function names */
    QString searchFilter() const;
    void setSearchFilter(QString searchFilter);

    /** Change the path of an existing folder and all its children */
    Q_INVOKABLE void setFolderPath(QString oldAbsPath, QString newRelPath);

    /** Create a new Function with the specified $type */
    Q_INVOKABLE quint32 createFunction(int type, QStringList fileList = QStringList());

    /** Return a reference to a Function with the specified $id */
    Q_INVOKABLE Function *getFunction(quint32 id);

    /** Return the associated qrc icon resource for the specified Function $type */
    Q_INVOKABLE QString functionIcon(int type);

    /** Enable/disable the Function preview feature */
    Q_INVOKABLE void setPreview(bool enable);

    /** Add $fID to the list of the currently selected Function IDs,
     *  considering $multiSelection as an append/replace action */
    Q_INVOKABLE void selectFunctionID(quint32 fID, bool multiSelection);

    /** Get the QML resource for a Function editor that can edit $funcID */
    Q_INVOKABLE QString getEditorResource(int funcID);

    /** Set $fID as the current Function ID being edited */
    Q_INVOKABLE void setEditorFunction(quint32 fID, bool requestUI, bool back);

    FunctionEditor *currentEditor() const;

    /** Returns if the UI is editing a Function */
    bool isEditing() const;

    /** Delete the list of Function IDs in $IDList. This happens AFTER a popup confirmation */
    Q_INVOKABLE void deleteFunctions(QVariantList IDList);

    /** Move the currently selected Function to the specified $newPath */
    Q_INVOKABLE void moveFunctions(QString newPath);

    /** Generic method to delete a list of item IDs specified in $list.
     *  This is used from within a Function editor and items can be of any type
     *  such as Functions, Fixtures, etc. as long as they have an ID.
     *  This happens AFTER a popup confirmation */
    Q_INVOKABLE void deleteEditorItems(QVariantList list);

    Q_INVOKABLE void renameFunctions(QVariantList IDList, QString newName, bool numbering, int startNumber, int digits);

    /** Returns the number of the currently selected Functions */
    int selectionCount() const;

    int sceneCount() const { return m_sceneCount; }
    int chaserCount() const { return m_chaserCount; }
    int sequenceCount() const { return m_sequenceCount; }
    int efxCount() const { return m_efxCount; }
    int collectionCount() const { return m_collectionCount; }
    int rgbMatrixCount() const { return m_rgbMatrixCount; }
    int scriptCount() const { return m_scriptCount; }
    int showCount() const { return m_showCount; }
    int audioCount() const { return m_audioCount; }
    int videoCount() const { return m_videoCount; }

    QStringList audioExtensions() const;
    QStringList pictureExtensions() const;
    QStringList videoExtensions() const;

    void setViewPosition(int viewPosition);
    int viewPosition() const;

protected:
    quint32 addFunctiontoDoc(Function *func, QString name, bool select);
    void addFunctionTreeItem(Function *func);
    void updateFunctionsTree();
    void clearTree();

signals:
    void functionsListChanged();
    void searchFilterChanged();
    void sceneCountChanged();
    void chaserCountChanged();
    void sequenceCountChanged();
    void efxCountChanged();
    void collectionCountChanged();
    void rgbMatrixCountChanged();
    void scriptCountChanged();
    void showCountChanged();
    void audioCountChanged();
    void videoCountChanged();
    void selectionCountChanged(int count);
    void isEditingChanged(bool editing);
    void viewPositionChanged(int viewPosition);

public slots:
    void slotDocLoaded();
    void slotFunctionAdded(quint32 fid);

private:
    /** Reference of the QML view */
    QQuickView *m_view;
    /** Reference of the project workspace */
    Doc *m_doc;
    /** Reference to the Functions tree model */
    TreeModel *m_functionTree;
    /** The QML ListView position in pixel for state restoring */
    int m_viewPosition;

    /** Flag that hold if Functions preview is enabled or not */
    bool m_previewEnabled;
    /** List of the Function IDs currently selected
     *  and previewed, if preview is enabled */
    QVariantList m_selectedIDList;

    quint32 m_filter;
    QString m_searchFilter;

    int m_sceneCount, m_chaserCount, m_sequenceCount, m_efxCount;
    int m_collectionCount, m_rgbMatrixCount, m_scriptCount;
    int m_showCount, m_audioCount, m_videoCount;

    FunctionEditor *m_currentEditor;
    FunctionEditor *m_sceneEditor;

    /*********************************************************************
     * DMX values (dumping and Scene editor)
     *********************************************************************/
public:

    /** Return the currently set channel values */
    QList <SceneValue> dumpValues() const;

    /** Reset the currently set channel values */
    void resetDumpValues();

    /** Dump DMX values provided by $dumpValues, filtered by the provided $selectedFixtures
     *  and the provided $channelMask.
     *  The new Scene will be named with $name if not empty, otherwise with an autogenerated name */
    void dumpOnNewScene(QList<SceneValue> dumpValues, QList<quint32> selectedFixtures,
                        quint32 channelMask, QString name);

    /** Dump DMX values provided by $dumpValues, filtered by the provided $selectedFixtures
     *  and the provided $channelMask. Values are dumped on an existing Scene with $sceneID */
    void dumpOnScene(QList<SceneValue> dumpValues, QList<quint32> selectedFixtures,
                     quint32 channelMask, quint32 sceneID);

    Q_INVOKABLE void setChannelValue(quint32 fxID, quint32 channel, uchar value);

protected:
    quint32 getChannelTypeMask(quint32 fxID, quint32 channel);
};

#endif // FUNCTIONMANAGER_H
