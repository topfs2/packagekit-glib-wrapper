/*
 *      Copyright (C) 2011 Tobias Arrskog
 *      https://github.com/topfs2/packagekit-glib-wrapper
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#include "PackageKitManager.h"
#include <iostream>

using namespace std;

class CPrintPackageCallback : public IPackageRetrieveCallback
{
public:
  virtual void Progress(unsigned int progress, unsigned int total)
  {
    cout << "Progress(" << progress << ", " << total << ");" << endl;
  }

  virtual void PushPackages(const std::vector<Package> &packages)
  {
    cout << "PushPackages()" << endl;
    for (unsigned int i = 0; i < packages.size(); i++)
      cout << "\t" << packages[i].ID << endl;
  }
};

class CPrintInstallCallback : public IInstallCallback
{
public:
  virtual void DownloadProgress(unsigned int progress, unsigned int total)
  {
    cout << "DownloadProgress(" << progress << ", " << total << ");" << endl;
  }

  virtual void InstallProgress(unsigned int progress, unsigned int total)
  {
    cout << "InstallProgress(" << progress << ", " << total << ");" << endl;
  }

  virtual void Done(bool success)
  {
    cout << "Done(" << success << ")" << endl;
  }
};

int main(int argc, char *argv[])
{
	if (! g_thread_supported ())
		g_thread_init (NULL);
	dbus_g_thread_init ();
	g_type_init ();


  CPackageKitManager manager;
  manager.Create();

  cout << "Started" << endl;

/*
  CPrintPackageCallback callback;
  manager.getPackages(&callback);
*/

  CPrintInstallCallback installCallback;
  manager.install("btanks;0.9.8083-2;amd64;maverick", &installCallback);

  manager.Join();
}
