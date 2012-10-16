/**
 * @licence app begin@
 * Copyright (C) 2011-2012  BMW AG
 *
 * This file is part of GENIVI Project Dlt Viewer.
 *
 * Contributions are licensed to the GENIVI Alliance under one or more
 * Contribution License Agreements.
 *
 * \copyright
 * This Source Code Form is subject to the terms of the
 * Mozilla Public License, v. 2.0. If a  copy of the MPL was not distributed with
 * this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \file plugininterface.h
 * For further information see http://www.genivi.org/.
 * @licence end@
 */

#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QString>
#include "dlt.h"
#include "qdlt.h"

#define PLUGIN_INTERFACE_VERSION "1.0.0"

//! Standard DLT Viewer Plugin Interface.
/*!
  This is the standard DLT Viewer Plugin Interface.
  This interface must be inherited by each DLT Viewer plugin.
*/

class QDLTPluginInterface
{
public:

    //! The name and identifier of the plugin.
    /*!
      The plugin must provide the name of the plugin.
      This name is used for identifie the plugin in the project configuration.
      \return The name of the plugin
    */
    virtual QString name() = 0;

    //! The description of the plugin.
    /*!
      The plugin can provide a more detailed description of the plugin.
      \return The description of the plugin
    */
    virtual QString description() = 0;

    //! The version number of the plugin.
    /*!
      The plugin has to return a version number with formt X.Y.Z.
      X counts up in case of real heavy changes (API changes or purpose changes)
      Y counts up when the module is reworked internally, functions are added etc
      Z counts up whenever a bug is fixed
      Recommondation: define <plugin name>_PLUGIN_VERSION "X.Y.Z" in your plugin header file.
      \return The version number of the plugin
    */
    virtual QString pluginVersion() = 0;

    //! The used plugin interface version number of the plugin.
    /*!
      The plugin has to return a version number of the used plugin interface.
      The plugin interface provides for this purpose the PLUGIN_INTERFACE_VERSION definition.
      \return The version number of the used plugin interface - PLUGIN_INTERFACE_VERSION in plugininterface.h
    */
    virtual QString pluginInterfaceVersion() = 0;

    //! The error message of the last plugin interface call.
    /*!
      The plugin can provide a error message of the last failed function call.
      \return Error message.
    */
    virtual QString error() = 0;

    //! Loading the configuration of the plugin.
    /*!
      The plugin can use a configuration stored in a file.
      The configuration can be a single file or a directory containing several files.
      Example of a configuration is an XML description file of the DLT non-verbose mode.
      Errors should be reported by providing error message.
      \sa error()
      \param filename This can be a filename or a directory name.
      \return True if the configuration is loaded succesful. False if there was any error or function was not supported.
    */
    virtual bool loadConfig(QString filename) = 0;

    //! Storing the configuration of the plugin.
    /*!
      The plugin can use a configuration stored in a file.
      The configuration can be a single file or a directory containing several files.
      Example of a configuration is an XML description file of the DLT non-verbose mode.
      Errors should be reported by providing error message.
      \sa error()
      \param filename This can be a filename or a directory name.
      \return True if the configuration is stored succesful. False if there was any error or function was not supported.
    */
    virtual bool saveConfig(QString filename) = 0;

    //! Providing detaild information about the loaded configuration.
    /*!
      The plugin can provide a detailed list of the loaded configuration.
      This is very useful to check, if the configuration is loaded succesfully.
      \return The detailed information about the loaded configuration. One string per configuration element should be used.
    */
    virtual QStringList infoConfig() = 0;


};

Q_DECLARE_INTERFACE(QDLTPluginInterface,
                    "org.genivi.DLT.Plugin.DLTPluginInterface/1.0");

//! Extended DLT Viewer Plugin Interface used by decoder plugins.
/*!
  This is an extended DLT Plugin Interface.
  This interface must be used by decoder plugins.
  DLT messages which are displayed are checked by the plugin, if they are valid and then decoded by the plugin.
*/
class QDLTPluginDecoderInterface
{
public:
    //! Check if the DLT message is handled by the plugin.
    /*!
      The plugin checks, if the DLT message is handled by the plugin.
      Errors should be reported by providing an error message.
      \sa QDLTPluginInterface::error()
      \param msg The current DLT message.
      \param triggeredByUser Reason for this method call was a user interaction with the GUI, e.g. clicked on "Export to ASCII" in the menu. 0 = not triggered by user, 1 = triggered by user
      \return True if the message is handled by the plugin. False if the message is not handled by the plugin.
    */
    virtual bool isMsg(QDltMsg &msg, int triggeredByUser) = 0;

    //! Decode the message and provide back the decoded message.
    /*!
      The plugin converts the DLT message.
      Errors should be reported by providing an error message.
      \sa QDLTPluginInterface::error()
      \param msg The current DLT message and the decoded message information.
      \param triggeredByUser Reason for this method call was a user interaction with the GUI, e.g. clicked on "Export to ASCII" in the menu. 0 = not triggered by user, 1 = triggered by user
      \return True if the message is converted by the plugin. False if the conversion fails.
    */
    virtual bool decodeMsg(QDltMsg &msg, int triggeredByUser) = 0;

};

Q_DECLARE_INTERFACE(QDLTPluginDecoderInterface,
                    "org.genivi.DLT.Plugin.DLTViewerPluginDecoderInterface/1.0");

//! Extended DLT Viewer Plugin Interface used by viewer plugins.
/*!
  This is an extended DLT Plugin Interface.
  This interface must be used by viewer plugins.
  The viewer plugin gets full access to the loaded DLT file.
*/
class QDltPluginViewerInterface
{
public:

    //! Initialise the viewer window.
    /*!
      The plugin manager provides the handle of the parent window.
      Errors should be reported by providing an error message.
      \sa QDLTPluginInterface::error()
      \return The form of the plugin.
    */
    virtual QWidget* initViewer() = 0;


    //! A new log file is opened by the DLT Viewer.
    /*! This function is called by the the viewer everytime a new log file is opened by the viewer
      or a new log file is created and before all messages were processed with initMsg and initMsgDecoded.
      Errors should be reported by providing an error message.
      \sa QDLTPluginInterface::error()
      \param file The DLT log file handle.
    */
    virtual void initFileStart(QDltFile *file) = 0;


    //! A new undecoded DLT message is processed after a new log file is opened by the DLT Viewer
    /*! After a new log file is opened this function is called by the viewer everytime
        a new undecoded message is processed.
      \sa QDLTPluginInterface::error()
      \param index The current DLT message index
      \param msg The current undecoded DLT message
    */
    virtual void initMsg(int index, QDltMsg &msg ) = 0;

    //! A new decoded DLT message is processed after a new log file is opened by the DLT Viewer
    /*! After a new log file is opened this function is called by the viewer everytime
        a new decoded message is processed.
      \sa QDLTPluginInterface::error()
      \param index The current DLT message index
      \param msg The current decoded DLT message
    */
    virtual void initMsgDecoded(int index, QDltMsg &msg ) = 0;

    //! A new log file was opened by the DLT Viewer.
    /*! This function is called by the the viewer everytime a new log file was opened by the viewer
      or a new log file is created and all messages were processed with initMsg and initMsgDecoded.
      Errors should be reported by providing an error message.
      \sa QDLTPluginInterface::error()
    */
    virtual void initFileFinish() = 0;


    //! A new message was received before updateMsg and updateMsgDecoded.
    /*! This function is called by the viewer everytime a new message was received
        before processing the message with updateMsg and updateMsgDecoded.
      Errors should be reported by providing an error message.
      \sa QDLTPluginInterface::error()
    */
    virtual void updateFileStart( ) = 0;


    //! New message were added to the log file.
    /*! This function is called when new log entries are added to the log file.
      Errors should be reported by providing an error message.
      \sa QDLTPluginInterface::error()
      \param index The current DLT message index
      \param msg The current undecoded DLT message
    */
    virtual void updateMsg(int index, QDltMsg &msg ) = 0;


    //! New message were added to the log file.
    /*! This function is called when new log entries are added to the log file.
      Errors should be reported by providing an error message.
      \sa QDLTPluginInterface::error()
      \param index The current DLT message index
      \param msg The current decoded DLT message
    */
    virtual void updateMsgDecoded(int index, QDltMsg &msg ) = 0;


    //! A new message was received after updateMsg and updateMsgDecoded.
    /*! This function is called by the viewer everytime a new message was received
        after processing the message with updateMsg and updateMsgDecoded.
      Errors should be reported by providing an error message.
      \sa QDLTPluginInterface::error()
    */
    virtual void updateFileFinish( ) = 0;


    //! An undecoded log message was selected to show more detailed information.
    /*!
      An undecoded log message was selected. The viewer plugin can show more detailed information about this plugin.
      \param index The current DLT message index
      \param msg The current undecoded DLT message
    */
    virtual void selectedIdxMsg(int index, QDltMsg &msg) = 0;


    //! A decoded log message was selected to show more detailed information.
    /*!
      A decoded log message was selected. The viewer plugin can show more detailed information about this plugin.
      \param index The current DLT message index
      \param msg The current decoded DLT message
    */
    virtual void selectedIdxMsgDecoded(int index, QDltMsg &msg) = 0;

};

Q_DECLARE_INTERFACE(QDltPluginViewerInterface,
                    "org.genivi.DLT.Plugin.DLTViewerPluginViewerInterface/1.0");

//! Extended DLT Control Plugin Interface used by control plugins.
/*!
  This is an extended DLT Plugin Interface.
  This interface must be used by control plugins.
  The control plugin interface can send control requests to the DLT daemon and receives control responses from the DLT daemon.
  The plugin gets informed about the available connections to DLT daemons.
*/
class QDltPluginControlInterface
{
public:

    //! A control interface is provided to the pluign.
    /*!
      This function is called once during initialisation of the plugin.
      Errors should be reported by providing an error message.
      \sa QDLTPluginInterface::error()
      \param control The control interface.
      \return True if everything went ok. False if there was an error.
    */
    virtual bool initControl(QDltControl *control) = 0;

    //! The connection table is initialised or changed.
    /*!
      This function is called by the the user changes the connection table to available targets.
      Errors should be reported by providing an error message.
      \sa QDLTPluginInterface::error()
      \param list The DLT log file handle.
      \return True if everything went ok. False if there was an error.
    */
    virtual bool initConnections(QStringList list) = 0;

    //! control message received.
    /*!
      A control message was received.
      Errors should be reported by providing an error message.
      \sa QDLTPluginInterface::error()
      \param index The connection where the ctrl message was received.
      \param msg The received control DLT message response.
      \return True if everything went ok. False if there was an error.
    */
    virtual bool controlMsg(int index, QDltMsg &msg) = 0;

    //! TCP connection state changed
    /*!
      The TCP connection state changed for a specific ECU item.

      \param ecuItem The ecuItem changed the connection state
      \param connectionState The new connection state of the ecuItem
      \return True if everything went ok. False if there was an error.
    */
    virtual bool stateChanged(int index, QDltConnection::QDltConnectionState connectionState) = 0;
};

Q_DECLARE_INTERFACE(QDltPluginControlInterface,
                    "org.genivi.DLT.Plugin.DLTViewerPluginControlInterface/1.0");

//! Extended DLT Command Plugin Interface
/*!
  This is an extended DLT Plugin Interface.
  This interface must be used by command plugins.
  The command plugin interface can be used to accept commands to execute from the UI or command line.
  The plugin receives commands from the DLT-viewer via this interface.
  This interface be used either synchronously or asynchronously. If you want a simple
  Synchronous call, just implement everything in command() and return 100 from
  the progress. Have your return value available on return of command.
  If you want an asynchronous interface, start a new thread in command and return progress
  information from progress(). Return 100 or more when the command is done.
*/
class QDltPluginCommandInterface
{
public:

    //! A command is executed in the plugin
    /*!
      This function is called when a request is made by the user to execute a command
      in the plugin. If the function returns false, more information about the error
      can be obtained via error()
      \sa QDLTPluginInterface::error()
      \param command The requested command.
      \param params List of parameters to the command.
      \return True if everything went ok. False if there was an error.
    */
    virtual bool command(QString command, QList<QString> params) = 0;

    //! Cancel the currently running command
    /*!
      When this function is called, the plugin should abort whatever it
      was doing and setting possible error and return values.
    */
    virtual void cancel() = 0;

    //! Return value from previous command.
    /*!
      After progress has been made to 100 or more, the will be called
      to get a return value from the previo0us command. The return value
      will be displayed to the user.
      \return Return value of the previous command
    */
    virtual QString commandReturnValue() = 0;

    //! Obtain progress information
    /*!
      Return the progress in the range 0 - 100. 100 means it is ready to return.
      A Progress dialog will be shown to the user while the command is processed.
      \return Progress information.
    */
    virtual int commandProgress() = 0;

    //! Get a list of available commands
    /*!
      Return a list of commands that are supported by this plugin.
      This is used primarily for the plugin UI to show a list of commands.
      \return List of commands
    */
    virtual QList<QString> commandList() = 0;
};

Q_DECLARE_INTERFACE(QDltPluginCommandInterface,
                    "org.genivi.DLT.Plugin.DLTViewerPluginCommandInterface/1.0");

#endif // PLUGININTERFACE_H
