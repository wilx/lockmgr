// Copyright (c) 2008, Václav Haisman
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#if ! defined (LOCKMANAGER_LOCKMGR_INTERNAL_GRAPH_HXX)
#define LOCKMANAGER_LOCKMGR_INTERNAL_GRAPH_HXX

// The following hunk basically does #undef min after including
// <windows.h>. That fixes compilation failure in implementation files
// of std::vector<bool> that use std::min() which otherwise gets
// substitued and thus fails to compile.
#if defined (WIN32)
#include "lockmgr/internal/windows.h"
#endif

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


//! Colour map used by DFS on RAG.
struct RAGColorMap
  : public boost::read_write_property_map_archetype<vertex_descr_type,
						    boost::default_color_type>
{
  //! Constructs RAGColorMap for given RAG.
  RAGColorMap (RAG & g)
    : rag (g)
  { }

  //! Reference to resource allocation graph.
  RAG & rag;
};


//! \brief Helper for the put() function of property_map interface of
//! lockmgr::RAGColorMap.
struct RAGNodeSetColorVisitor
  : public boost::static_visitor<void>
{
  //! \param c Colour to be set.
  RAGNodeSetColorVisitor (RAGColorMap::value_type c)
    : color (c)
  { }

  //! Function call operator that sets colour to node.
  result_type 
  operator () (NodeBase & node) const
  {
    node.color = color;
  }
  
  //! Colour to be set on visited node.
  RAGColorMap::value_type const color;
};


//! \brief Helper for the get() function of property_map interface of
//! lockmgr::RAGColorMap.
struct RAGNodeGetColorVisitor
  : public boost::static_visitor<RAGColorMap::value_type>
{
  //! Function call operator that returns colour of give node.
  result_type 
  operator () (NodeBase const & node) const
  {
    return static_cast<result_type>(node.color);
  }
};


//! \brief Put() function for property_map interface of
//! lockmgr::RAGColorMap.
inline 
void
put (lockmgr::RAGColorMap & map, lockmgr::RAGColorMap::key_type vertex,
     lockmgr::RAGColorMap::value_type color)
{
  lockmgr::RAGNode node (get (boost::vertex_name, map.rag, vertex));
  apply_visitor (lockmgr::RAGNodeSetColorVisitor (color), node);
  put (boost::vertex_name, map.rag, vertex, node);
}


//! \brief Get() function for property_map interface of
//! lockmgr::RAGColorMap.
inline
lockmgr::RAGColorMap::value_type
get (lockmgr::RAGColorMap const & map, lockmgr::RAGColorMap::key_type vertex)
{
  lockmgr::RAGNode const & node = get (boost::vertex_name, map.rag, vertex);
  return static_cast<lockmgr::RAGColorMap::value_type>
    (apply_visitor (lockmgr::RAGNodeGetColorVisitor (), node));
}


} // namespace lockmgr

#endif // LOCKMANAGER_LOCKMGR_INTERNAL_GRAPH_HXX
