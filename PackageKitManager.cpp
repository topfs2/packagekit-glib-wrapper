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

CPackageKitManager::CPackageKitManager() : CThread()
{
  m_loop = g_main_loop_new (NULL, FALSE);

  m_client = pk_client_new ();
}

struct PackageFetchUserData
{
  IPackageRetrieveCallback *callback;
  std::vector<Package> packages;
};

struct PackageInstallUserData
{
  IInstallCallback *callback;
};

void CPackageKitManager::getPackages(IPackageRetrieveCallback *callback)
{
  PackageFetchUserData *user_data = new PackageFetchUserData;
  user_data->callback = callback;

  pk_client_get_packages_async(m_client,
                               PK_FILTER_ENUM_GUI,
                               NULL,
                               CPackageKitManager::GetPackagesProgressCallback,
                               user_data,
                               CPackageKitManager::GetPackagesFinishCallback,
                               user_data);
}

void CPackageKitManager::install(const string &ID, IInstallCallback *callback)
{
  PackageInstallUserData *user_data = new PackageInstallUserData;
  user_data->callback = callback;

  gchar **packages = new gchar *[1];
  packages[0] = g_strdup_printf("%s", ID.c_str());

  pk_client_install_packages_async(m_client,
                                   TRUE,
                                   packages,
                                   NULL,
                                   CPackageKitManager::InstallProgressCallback,
                                   user_data,
                                   CPackageKitManager::InstallFinishCallback,
                                   user_data);

  g_free(packages[0]);
  delete packages;
}

void CPackageKitManager::Process()
{
	g_main_loop_run(m_loop);
}

void CPackageKitManager::InstallProgressCallback(PkProgress *progress, PkProgressType type, gpointer user_data)
{
  PackageInstallUserData *data = (PackageInstallUserData *)user_data;
	gint percentage;

#ifdef DEBUG_CALLBACKS
	switch (type)
	{
		case PK_PROGRESS_TYPE_PACKAGE_ID:
			cout << "PACKAGE_ID" << endl;
			break;
		case PK_PROGRESS_TYPE_TRANSACTION_ID:
			cout << "TRANSACTION_ID" << endl;
			break;
		case PK_PROGRESS_TYPE_PERCENTAGE:
			cout << "PERCENTAGE" << endl;
			break;
		case PK_PROGRESS_TYPE_SUBPERCENTAGE:
			cout << "SUBPERCENTAGE" << endl;
			break;
		case PK_PROGRESS_TYPE_ALLOW_CANCEL:
			cout << "ALLOW_CANCEL" << endl;
			break;
		case PK_PROGRESS_TYPE_STATUS:
			cout << "STATUS" << endl;
			break;
		case PK_PROGRESS_TYPE_ROLE:
			cout << "ROLE " << endl;
			break;
		case PK_PROGRESS_TYPE_CALLER_ACTIVE:
			cout << "CALLER_ACTIVE" << endl;
			break;
		case PK_PROGRESS_TYPE_ELAPSED_TIME:
			cout << "ELAPSED_TIME" << endl;
			break;
		case PK_PROGRESS_TYPE_REMAINING_TIME:
			cout << "REMAINING_TIME" << endl;
			break;
		case PK_PROGRESS_TYPE_SPEED:
			cout << "SPEED" << endl;
			break;
		case PK_PROGRESS_TYPE_UID:
			cout << "UID" << endl;
			break;
		case PK_PROGRESS_TYPE_PACKAGE:
			cout << "PACKAGE" << endl;
			break;
		case PK_PROGRESS_TYPE_INVALID:
			cout << "INVALID" << endl;
			break;
	}
#endif

	switch (type)
	{
		case PK_PROGRESS_TYPE_PERCENTAGE:
		  g_object_get (progress, "percentage", &percentage, NULL);
      if (percentage >= 0 && percentage <= 100)
        data->callback->InstallProgress(percentage, 100);
      else
        cout << "Wrong percentage " << percentage << endl;
			break;

    default:
      break;
	}
}

void CPackageKitManager::InstallFinishCallback(GObject *object, GAsyncResult *res, gpointer user_data)
{
  PackageInstallUserData *data = (PackageInstallUserData *)user_data;

  data->callback->Done(true);

  delete data;
}

void CPackageKitManager::GetPackagesProgressCallback(PkProgress *progress, PkProgressType type, gpointer user_data)
{
  PackageFetchUserData *data = (PackageFetchUserData *)user_data;

  static gchar *package_id = NULL;
	gint percentage;

#ifdef DEBUG_CALLBACKS
	switch (type)
	{
		case PK_PROGRESS_TYPE_PACKAGE_ID:
			cout << "PACKAGE_ID" << endl;
			break;
		case PK_PROGRESS_TYPE_TRANSACTION_ID:
			cout << "TRANSACTION_ID" << endl;
			break;
		case PK_PROGRESS_TYPE_PERCENTAGE:
			cout << "PERCENTAGE" << endl;
			break;
		case PK_PROGRESS_TYPE_SUBPERCENTAGE:
			cout << "SUBPERCENTAGE" << endl;
			break;
		case PK_PROGRESS_TYPE_ALLOW_CANCEL:
			cout << "ALLOW_CANCEL" << endl;
			break;
		case PK_PROGRESS_TYPE_STATUS:
			cout << "STATUS" << endl;
			break;
		case PK_PROGRESS_TYPE_ROLE:
			cout << "ROLE " << endl;
			break;
		case PK_PROGRESS_TYPE_CALLER_ACTIVE:
			cout << "CALLER_ACTIVE" << endl;
			break;
		case PK_PROGRESS_TYPE_ELAPSED_TIME:
			cout << "ELAPSED_TIME" << endl;
			break;
		case PK_PROGRESS_TYPE_REMAINING_TIME:
			cout << "REMAINING_TIME" << endl;
			break;
		case PK_PROGRESS_TYPE_SPEED:
			cout << "SPEED" << endl;
			break;
		case PK_PROGRESS_TYPE_UID:
			cout << "UID" << endl;
			break;
		case PK_PROGRESS_TYPE_PACKAGE:
			cout << "PACKAGE" << endl;
			break;
		case PK_PROGRESS_TYPE_INVALID:
			cout << "INVALID" << endl;
			break;
	}
#else
	switch (type)
	{
		case PK_PROGRESS_TYPE_PACKAGE_ID:
      g_object_get (progress, "package-id", &package_id, NULL);
      data->packages.push_back((Package){ package_id });
			break;
			break;
		case PK_PROGRESS_TYPE_PERCENTAGE:
		  g_object_get (progress, "percentage", &percentage, NULL);
      if (percentage > 0 && percentage < 100)
        data->callback->Progress(percentage, 100);
			break;
    default:
      break;
	}
#endif
}

void CPackageKitManager::GetPackagesFinishCallback(GObject *object, GAsyncResult *res, gpointer user_data)
{
  PackageFetchUserData *data = (PackageFetchUserData *)user_data;

  data->callback->PushPackages(data->packages);

  delete data;
}
