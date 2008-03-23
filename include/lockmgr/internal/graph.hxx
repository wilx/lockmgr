#if ! defined (LOCKMANAGER_LOCKMGR_INTERNAL_GRAPH_HXX)
#define LOCKMANAGER_LOCKMGR_INTERNAL_GRAPH_HXX

#include "lockmgr/internal/windows.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include "lockmgr/internal/nodes.hxx"

namespace lockmgr
{

//! Vertex property is RAGNode.
typedef boost::property<
  boost::vertex_name_t, 
  RAGNode
  > 
rag_vertex_property_type;


//! Type of Resource Allocation Graph.
typedef boost::adjacency_list<
  boost::vecS,
  boost::vecS,
  boost::directedS,
  rag_vertex_property_type
  > 
RAG;

  
//! Type of vertex descriptor of Boost's graph.
typedef boost::graph_traits<RAG>::vertex_descriptor vertex_descr_type;


} // namespace lockmgr

#endif // LOCKMANAGER_LOCKMGR_INTERNAL_GRAPH_HXX
