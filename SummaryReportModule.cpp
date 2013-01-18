/*
 * The Sleuth Kit
 *
 * Contact: Brian Carrier [carrier <at> sleuthkit [dot] org]
 * Copyright (c) 2011-2012 Basis Technology Corporation. All Rights
 * reserved.
 *
 * This software is distributed under the Common Public License 1.0
 */

/** 
 * \file SummaryReportModule.cpp 
 * Contains the implementation of a post-processing module that creates a blackboard artifacts report.
 */

// Module includes
#include "SummaryReport.h"

// TSK Framework includes
#include "TskModuleDev.h"

// Poco includes
#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/Exception.h"

// C/C++ standard library includes 
#include <string>
#include <sstream>
#include <vector>

extern "C" 
{
    /**
     * Module identification function. 
     *
     * @return The name of the module.
     */
    TSK_MODULE_EXPORT const char *name()
    {
        return "SummaryReport";
    }

    /**
     * Module identification function. 
     *
     * @return A description of the module.
     */
    TSK_MODULE_EXPORT const char *description()
    {
        return "Creates an HTML report on data posted to the blackboard";
    }

    /**
     * Module identification function. 
     *
     * @return The version of the module.
     */
    TSK_MODULE_EXPORT const char *version()
    {
        return "1.0.0";
    }

    /**
     * Module initialization function. Receives a string of initialization arguments, 
     * typically read by the caller from a pipeline configuration file. 
     * Returns TskModule::OK or TskModule::FAIL. Returning TskModule::FAIL indicates 
     * the module is not in an operational state.  
     *
     * @param args a string of initialization arguments.
     * @return TskModule::OK if initialization succeeded, otherwise TskModule::FAIL.
     */
    TskModule::Status TSK_MODULE_EXPORT initialize(const char* arguments)
    {    
        return TskModule::OK;
    }

    /**
     * Module execution function. 
     *
     * @returns TskModule::OK on success, TskModule::FAIL on error, or TskModule::STOP.
     */
    TskModule::Status TSK_MODULE_EXPORT report() 
    {
        std::ostringstream msgPrefix;
        msgPrefix << name() << "::report : ";
        try
        {
            // Create an output folder.
            Poco::Path outputFolderPath = Poco::Path::forDirectory(GetSystemProperty(TskSystemProperties::MODULE_OUT_DIR));
            outputFolderPath.pushDirectory(name());
            Poco::File(outputFolderPath).createDirectory();

            // Generate the report.
            outputFolderPath.setFileName("SummaryReport.htm");
            TskSummaryReport::generateReport(outputFolderPath.toString());

            return TskModule::OK;
        }
        catch (TskException &ex)
        {
            std::ostringstream msg;
            msg << msgPrefix.str() << "TskException: " << ex.message();
            LOGERROR(msg.str());
            return TskModule::FAIL;
        }
        catch (Poco::Exception &ex)
        {
            std::ostringstream msg;
            msg << msgPrefix.str() << "Poco::Exception: " << ex.displayText();
            LOGERROR(msg.str());
            return TskModule::FAIL;
        }
        catch (std::exception &ex)
        {
            std::ostringstream msg;
            msg << msgPrefix.str() << "std::exception: " << ex.what();
            LOGERROR(msg.str());
            return TskModule::FAIL;
        }
        catch (...)
        {
            LOGERROR(msgPrefix.str() + "unrecognized exception");
            return TskModule::FAIL;
        }
    }

    /**
     * Module cleanup function. This is where the module should free any resources 
     * allocated during initialization or execution.
     *
     * @returns TskModule::OK on success and TskModule::FAIL on error.
     */
    TskModule::Status TSK_MODULE_EXPORT finalize()
    {
        std::ostringstream msgPrefix;
        msgPrefix << name() << "::finalize : ";
        try
        {
            // Delete the output folder if it's empty.
            Poco::Path outputFolderPath = Poco::Path::forDirectory(GetSystemProperty(TskSystemProperties::MODULE_OUT_DIR));
            outputFolderPath.pushDirectory(name());
            Poco::File outputFolder(outputFolderPath);
            std::vector<Poco::File> filesList;
            outputFolder.list(filesList);
            if (filesList.empty())
            {
                outputFolder.remove(true);
            }

            return TskModule::OK;
        }
        catch (TskException &ex)
        {
            std::ostringstream msg;
            msg << msgPrefix.str() << "TskException: " << ex.message();
            LOGERROR(msg.str());
            return TskModule::FAIL;
        }
        catch (Poco::Exception &ex)
        {
            std::ostringstream msg;
            msg << msgPrefix.str() << "Poco::Exception: " << ex.displayText();
            LOGERROR(msg.str());
            return TskModule::FAIL;
        }
        catch (std::exception &ex)
        {
            std::ostringstream msg;
            msg << msgPrefix.str() << "std::exception: " << ex.what();
            LOGERROR(msg.str());
            return TskModule::FAIL;
        }
        catch (...)
        {
            LOGERROR(msgPrefix.str() + "unrecognized exception");
            return TskModule::FAIL;
        }
    }
}

