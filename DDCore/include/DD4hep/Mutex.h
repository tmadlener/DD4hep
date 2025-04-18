//==========================================================================
//  AIDA Detector description implementation 
//--------------------------------------------------------------------------
// Copyright (C) Organisation europeenne pour la Recherche nucleaire (CERN)
// All rights reserved.
//
// For the licensing terms see $DD4hepINSTALL/LICENSE.
// For the list of contributors see $DD4hepINSTALL/doc/CREDITS.
//
// Author     : M.Frank
//
//==========================================================================

#ifndef DD4HEP_MUTEX_H
#define DD4HEP_MUTEX_H

// C/C++ include files
#include <mutex>

/// Namespace for the AIDA detector description toolkit
namespace dd4hep {
  typedef std::recursive_mutex            dd4hep_mutex_t;
  typedef std::lock_guard<dd4hep_mutex_t> dd4hep_lock_t;
}
#endif  // DD4HEP_MUTEX_H
