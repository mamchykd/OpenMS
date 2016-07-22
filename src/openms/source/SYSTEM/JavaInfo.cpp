// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2016.
//
// This software is released under a three-clause BSD license:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of any author or any participating institution
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// For a full list of authors, refer to the file AUTHORS.
// --------------------------------------------------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL ANY OF THE AUTHORS OR THE CONTRIBUTING
// INSTITUTIONS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// --------------------------------------------------------------------------
// $Maintainer: Timo Sachsenberg $
// $Authors: Timo Sachsenberg $
// --------------------------------------------------------------------------

#include <OpenMS/SYSTEM/JavaInfo.h>

#include <OpenMS/CONCEPT/LogStream.h>
#include <OpenMS/DATASTRUCTURES/String.h>
#include <OpenMS/SYSTEM/File.h>

#include <QtCore/QProcess>
#include <QtCore/QDir>

namespace OpenMS
{

  bool JavaInfo::canRun(const String& java_executable, bool verbose_on_error)
  {
    //
    // first check the file exists
    //
    try
    {
      // throws exception if not found
      File::find(java_executable);
    }
    catch (...)
    {
      if (verbose_on_error)
      {
        LOG_ERROR << "Java-Check:"
          << "  Java not found at '" << java_executable << "'!\n"
          << "  Make sure Java is installed and this location is correct.\n";
        if (QDir::isRelativePath(java_executable.toQString()))
        {
          static String path;
          if (path.empty())
          {
            path = getenv("PATH");
          }
          LOG_ERROR << "  You might need to add the Java binary to your PATH variable\n"
                    << "  or use an absolute path+filename pointing to Java.\n" 
                    << "  The current SYSTEM PATH is: '" << path << "'.\n\n"
#ifdef __APPLE__
                    << "  On MacOSX, application bundles change the system PATH; Use an abolute path to Java or open your exectuable (e.g. KNIME/TOPPAS/TOPPView) from within the bundle!\n"
#endif
                    << std::endl;
        }
        else 
        {
          LOG_ERROR << "  You gave an absolute path to Java. Please check if it's correct.\n"
                    << "  You can also try 'java' if your system path is correctly configured.\n"
                    << std::endl;
        }

        return false;
      }
    }
    
    //
    // then, check if it can be run
    //
    QProcess qp;
    qp.start(java_executable.toQString(), QStringList() << "-version", QIODevice::ReadOnly);
    bool success = qp.waitForFinished();
    if (!success && verbose_on_error)
    {
      LOG_ERROR << "Java-Check:"
                << "  Java was found at '" << java_executable << "' but cannot be executed or the process timed out (can happen on very busy systems).\n"
                << "  Please fix permissions or if your system is under heavy load, set the TOPP tools 'force' flag in order to avoid this check." << std::endl;
    }
    return success;
  }

} // namespace OpenMS
