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

// Framework include files
#include <JSON/Printout.h>
#include <JSON/Elements.h>

// C/C++ include files
#include <cstdio>
#include <stdexcept>
#include <string>
#include <stdexcept>

using namespace dd4hep::json;
static const size_t INVALID_NODE = ~0U;

// Forward declarations
namespace dd4hep {
  std::pair<int, double> _toInteger(const std::string& value);
  std::pair<int, double> _toFloatingPoint(const std::string& value);
  void   _toDictionary(const std::string& name, const std::string& value, const std::string& typ);
  std::string _getEnviron(const std::string& env);
}
// Static storage
namespace {
  std::string _checkEnviron(const std::string& env)  {
    std::string r = dd4hep::_getEnviron(env);
    return r.empty() ? env : r;
  }
}

namespace {

  // This should ensure we are not passing temporaries of std::string and then
  // returning the "const char*" content calling .c_str()
  const ptree::data_type& value_data(const ptree& entry)  {
    return entry.data();
  }

  JsonElement* node_first(JsonElement* e, const char* tag) {
    if ( e )  {
      std::string t(tag);
      if ( t == "*" )  {
        ptree::iterator i = e->second.begin();
        return i != e->second.end() ? &(*i) : 0;
      }
      ptree::assoc_iterator i = e->second.find(t);
      return i != e->second.not_found() ? &(*i) : 0;
    }
    return 0;
  }

  size_t node_count(JsonElement* e, const std::string& t) {
    return e ? (t=="*" ? e->second.size() : e->second.count(t)) : 0;
  }

  Attribute attribute_node(JsonElement* n, const char* t)  {
    if ( n )  {
      auto i = n->second.find(t);
      return i != n->second.not_found() ? &(*i) : 0;
    }
    return 0;
  }

  const char* attribute_value(Attribute a) {
    return value_data(a->second).c_str();
  }
}

std::string dd4hep::json::_toString(Attribute attr) {
  if (attr)
    return _toString(attribute_value(attr));
  return "";
}

template <typename T> static inline std::string __to_string(T value, const char* fmt) {
  char text[128];
  ::snprintf(text, sizeof(text), fmt, value);
  return text;
}

/// Do-nothing version. Present for completeness and argument interchangeability
std::string dd4hep::json::_toString(const char* s) {
  if ( !s || *s == 0 ) return "";
  else if ( !(*s == '$' && *(s+1) == '{') ) return s;
  return _checkEnviron(s);
}

/// Do-nothing version. Present for completeness and argument interchangeability
std::string dd4hep::json::_toString(const std::string& s) {
  if ( s.length() < 3 || s[0] != '$' ) return s;
  else if ( !(s[0] == '$' && s[1] == '{') ) return s;
  return _checkEnviron(s);
}

/// Format unsigned long integer to string with arbitrary format
std::string dd4hep::json::_toString(unsigned long v, const char* fmt) {
  return __to_string(v, fmt);
}

/// Format unsigned integer (32 bits) to string with arbitrary format
std::string dd4hep::json::_toString(unsigned int v, const char* fmt) {
  return __to_string(v, fmt);
}

/// Format signed integer (32 bits) to string with arbitrary format
std::string dd4hep::json::_toString(int v, const char* fmt) {
  return __to_string(v, fmt);
}

/// Format signed long integer to string with arbitrary format
std::string dd4hep::json::_toString(long v, const char* fmt)   {
  return __to_string(v, fmt);
}

/// Format single procision float number (32 bits) to string with arbitrary format
std::string dd4hep::json::_toString(float v, const char* fmt) {
  return __to_string(v, fmt);
}

/// Format double procision float number (64 bits) to string with arbitrary format
std::string dd4hep::json::_toString(double v, const char* fmt) {
  return __to_string(v, fmt);
}

/// Format pointer to string with arbitrary format
std::string dd4hep::json::_ptrToString(const void* v, const char* fmt) {
  return __to_string(v, fmt);
}

long dd4hep::json::_toLong(const char* value) {
  return value ? (long)dd4hep::_toInteger(_toString(value)).second : -1L;
}

int dd4hep::json::_toInt(const char* value) {
  return value ? (int)dd4hep::_toInteger(_toString(value)).second : -1;
}

bool dd4hep::json::_toBool(const char* value) {
  if (value) {
    std::string s = _toString(value);
    return s == "true";
  }
  return false;
}

float dd4hep::json::_toFloat(const char* value) {
  return (float)(value ? dd4hep::_toFloatingPoint(_toString(value)).second : 0.0);
}

double dd4hep::json::_toDouble(const char* value) {
  return value ? dd4hep::_toFloatingPoint(_toString(value)).second : 0.0;
}

void dd4hep::json::_toDictionary(const char* name, const char* value) {
  dd4hep::_toDictionary(name, value, "number");
}

template <typename T> void dd4hep::json::_toDictionary(const char* name, T value)   {
  dd4hep::_toDictionary(name, _toString(value), "number");
}

template void dd4hep::json::_toDictionary(const char* name, const std::string& value);
template void dd4hep::json::_toDictionary(const char* name, unsigned long value);
template void dd4hep::json::_toDictionary(const char* name, unsigned int value);
template void dd4hep::json::_toDictionary(const char* name, unsigned short value);
template void dd4hep::json::_toDictionary(const char* name, int value);
template void dd4hep::json::_toDictionary(const char* name, long value);
template void dd4hep::json::_toDictionary(const char* name, short value);
template void dd4hep::json::_toDictionary(const char* name, float value);
template void dd4hep::json::_toDictionary(const char* name, double value);

/// Evaluate string constant using environment stored in the evaluator
std::string dd4hep::json::getEnviron(const std::string& env)   {
  return dd4hep::_getEnviron(env);
}

/// Copy constructor
NodeList::NodeList(const NodeList& copy)
  : m_tag(copy.m_tag), m_node(copy.m_node)
{
  reset();
}

/// Initializing constructor
NodeList::NodeList(JsonElement* node, const std::string& tag_value)
  : m_tag(tag_value), m_node(node)
{
  reset();
}

/// Default destructor
NodeList::~NodeList() {
}

/// Reset the nodelist
JsonElement* NodeList::reset() {
  if ( m_tag == "*" )
    m_ptr = std::make_pair(m_node->second.ordered_begin(), m_node->second.not_found());
  else
    m_ptr = m_node->second.equal_range(m_tag);
  if ( m_ptr.first != m_ptr.second )
    return &(*m_ptr.first);
  return 0;
}

/// Advance to next element
JsonElement* NodeList::next() const {
  if ( m_ptr.first != m_ptr.second )  {
    m_ptr.first = ++m_ptr.first;
    if ( m_ptr.first != m_ptr.second ) return &(*m_ptr.first);
  }
  return 0;
}

/// Go back to previous element
JsonElement* NodeList::previous() const {
  if ( m_ptr.first != m_ptr.second )  {
    m_ptr.first = --m_ptr.first;
    if ( m_ptr.first != m_ptr.second ) return &(*m_ptr.first);
  }
  return 0;
}

/// Assignment operator
NodeList& NodeList::operator=(const NodeList& l) {
  if ( this != &l ) {
    m_tag  = l.m_tag;
    m_node = l.m_node;
    reset();
  }
  return *this;
}

/// Unicode text access to the element's tag. This must be wrong ....
const char* Handle_t::rawTag() const {
  return m_node->first.c_str();
}

/// Unicode text access to the element's text
const char* Handle_t::rawText() const {
  return value_data(m_node->second).c_str();
}

/// Unicode text access to the element's value
const char* Handle_t::rawValue() const {
  return value_data(m_node->second).c_str();
}

/// Access attribute pointer by the attribute's unicode name (no exception thrown if not present)
Attribute Handle_t::attr_nothrow(const char* tag_value) const {
  return attribute_node(m_node, tag_value);
}

/// Check for the existence of a named attribute
bool Handle_t::hasAttr(const char* tag_value) const {
  return m_node && 0 != node_first(m_node, tag_value);
}

/// Retrieve a collection of all attributes of this DOM element
std::vector<Attribute> Handle_t::attributes() const {
  std::vector < Attribute > attrs;
  if (m_node) {
    for(ptree::iterator i=m_node->second.begin(); i!=m_node->second.end(); ++i)  {
      Attribute a = &(*i);
      attrs.emplace_back(a);
    }
  }
  return attrs;
}

size_t Handle_t::numChildren(const char* t, bool throw_exception) const {
  size_t n = node_count(m_node, t);
  if (n == INVALID_NODE && !throw_exception)
    return 0;
  else if (n != INVALID_NODE)
    return n;
  std::string msg = "Handle_t::numChildren: ";
  if (m_node)
    msg += "Element [" + tag() + "] has no children of type '" + _toString(t) + "'";
  else
    msg += "Element [INVALID] has no children of type '" + _toString(t) + "'";
  throw std::runtime_error(msg);
}

/// Remove a single child node identified by its handle from the tree of the element
Handle_t Handle_t::child(const char* t, bool throw_exception) const {
  Elt_t e = node_first(m_node, t);
  if (e || !throw_exception)
    return e;
  std::string msg = "Handle_t::child: ";
  if (m_node)
    msg += "Element [" + tag() + "] has no child of type '" + _toString(t) + "'";
  else
    msg += "Element [INVALID]. Cannot remove child of type: '" + _toString(t) + "'";
  throw std::runtime_error(msg);
}

NodeList Handle_t::children(const char* tag_value) const {
  return NodeList(m_node, tag_value);
}

bool Handle_t::hasChild(const char* tag_value) const {
  return node_first(m_node, tag_value) != 0;
}

/// Access attribute pointer by the attribute's unicode name (throws exception if not present)
Attribute Handle_t::attr_ptr(const char* t) const {
  Attribute a = attribute_node(m_node, t);
  if (0 != a)
    return a;
  std::string msg = "Handle_t::attr_ptr: ";
  if (m_node)
    msg += "Element [" + tag() + "] has no attribute of type '" + _toString(t) + "'";
  else
    msg += "Element [INVALID] has no attribute of type '" + _toString(t) + "'";
  throw std::runtime_error(msg);
}

/// Access attribute name (throws exception if not present)
const char* Handle_t::attr_name(const Attribute a) const {
  if (a) {
    return a->first.c_str();
  }
  throw std::runtime_error("Attempt to access invalid XML attribute object!");
}

/// Access attribute value by the attribute's unicode name (throws exception if not present)
const char* Handle_t::attr_value(const char* attr_tag) const {
  return attribute_value(attr_ptr(attr_tag));
}

/// Access attribute value by the attribute  (throws exception if not present)
const char* Handle_t::attr_value(const Attribute attr_val) const {
  return attribute_value(attr_val);
}

/// Access attribute value by the attribute's unicode name (no exception thrown if not present)
const char* Handle_t::attr_value_nothrow(const char* attr_tag) const {
  Attribute a = attr_nothrow(attr_tag);
  return a ? attribute_value(a) : 0;
}


/// Access the ROOT eleemnt of the DOM document
Handle_t Document::root() const   {
  if ( m_doc )   {
    return m_doc;
  }
  throw std::runtime_error("Document::root: Invalid handle!");
}

/// Assign new document. Old document is dropped.
DocumentHolder& DocumentHolder::assign(DOC d)   {
  if ( m_doc )   {
    printout(DEBUG,"DocumentHolder","+++ Release JSON document....");
    delete m_doc;
  }
  m_doc = d;
  return *this;
}

/// Standard destructor - releases the document
DocumentHolder::~DocumentHolder()   {
  assign(0);
}

Attribute Element::getAttr(const char* name) const {
  return m_element ? attribute_node(m_element, name) : 0;
}

Collection_t::Collection_t(Handle_t element, const char* tag_value)
  : m_children(element, tag_value) {
  m_node = m_children.reset();
}

/// Constructor over XmlElements in a node list
Collection_t::Collection_t(NodeList node_list)
  : m_children(node_list) {
  m_node = m_children.reset();
}

/// Reset the collection object to restart the iteration
Collection_t& Collection_t::reset() {
  m_node = m_children.reset();
  return *this;
}

/// Access the collection size. Avoid this call -- sloooow!
size_t Collection_t::size() const {
  return Handle_t(m_children.m_node).numChildren(m_children.m_tag.c_str(), false);
}

/// Helper function to throw an exception
void Collection_t::throw_loop_exception(const std::exception& e) const {
  if (m_node) {
    throw std::runtime_error(std::string(e.what()) + "\n" + "dd4hep: Error interpreting XML nodes of type <" + tag() + "/>");
  }
  throw std::runtime_error(std::string(e.what()) + "\n" + "dd4hep: Error interpreting collections XML nodes.");
}

void Collection_t::operator++() const {
  while (m_node) {
    m_node = m_children.next();
    if (m_node && m_node->second.size() > 0 )
      return;
  }
}

void Collection_t::operator--() const {
  while (m_node) {
    m_node = m_children.previous();
    if (m_node && m_node->second.size() > 0 )
      return;
  }
}

void Collection_t::operator++(int) const {
  ++(*this);
}

void Collection_t::operator--(int) const {
  --(*this);
}

void dd4hep::json::dumpTree(Handle_t elt)   {
  dumpTree(elt.ptr());
}

void dd4hep::json::dumpTree(Element elt)   {
  dumpTree(elt.ptr());
}

void dd4hep::json::dumpTree(const JsonElement* elt)   {
  struct Dump {
    void operator()(const JsonElement* e, const std::string& tag)   const  {
      std::string t = tag+"   ";
      printout(INFO,"DumpTree","+++ %s %s: %s",tag.c_str(), e->first.c_str(), e->second.data().c_str());
      for(auto i=e->second.begin(); i!=e->second.end(); ++i)
        (*this)(&(*i), t);
    }
  } _dmp;
  _dmp(elt," ");
}

