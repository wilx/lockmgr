#if ! defined (LOCKMANAGER_LOCKMGR_INTERNAL_GRAPH_HXX)
#define LOCKMANAGER_LOCKMGR_INTERNAL_GRAPH_HXX

#include "lockmgr/internal/windows.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include "lockmgr/internal/nodes.hxx"

namespace lockmgr
{

//! Vertex name property is RAGNode.
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


//! Type of vertex descriptor of Resource Allocation Graph.
typedef boost::graph_traits<RAG>::vertex_descriptor vertex_descr_type;

//! Type of edge descriptor of Resource Allocation Graph.
typedef boost::graph_traits<RAG>::edge_descriptor edge_descr_type;


//!
struct RAGColorMap
  : public boost::read_write_property_map_archetype<vertex_descr_type,
						    boost::default_color_type>
{
  RAGColorMap (RAG & g)
    : rag (g)
  { }

  RAG & rag;
};

} // namespace lockmgr

namespace boost
{

inline 
void
put (lockmgr::RAGColorMap & map, lockmgr::RAGColorMap::key_type vertex,
     lockmgr::RAGColorMap::value_type color)
{
  lockmgr::RAGNode node (get (vertex_name, map.rag, vertex));
  node.base.color = color;
  put (vertex_name, map.rag, vertex, node);
}


inline
lockmgr::RAGColorMap::value_type
get (lockmgr::RAGColorMap const & map, lockmgr::RAGColorMap::key_type vertex)
{
  return static_cast<lockmgr::RAGColorMap::value_type>
    (get (vertex_name, map.rag, vertex).base.color);
}

} // namespace boost

#endif // LOCKMANAGER_LOCKMGR_INTERNAL_GRAPH_HXX
