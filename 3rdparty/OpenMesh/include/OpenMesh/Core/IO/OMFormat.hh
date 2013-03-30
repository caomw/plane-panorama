/*===========================================================================*\
 *                                                                           *
 *                               OpenMesh                                    *
 *      Copyright (C) 2001-2011 by Computer Graphics Group, RWTH Aachen      *
 *                           www.openmesh.org                                *
 *                                                                           *
 *---------------------------------------------------------------------------*
 *  This file is part of OpenMesh.                                           *
 *                                                                           *
 *  OpenMesh is free software: you can redistribute it and/or modify         *
 *  it under the terms of the GNU Lesser General Public License as           *
 *  published by the Free Software Foundation, either version 3 of           *
 *  the License, or (at your option) any later version with the              *
 *  following exceptions:                                                    *
 *                                                                           *
 *  If other files instantiate templates or use macros                       *
 *  or inline functions from this file, or you compile this file and         *
 *  link it with other files to produce an executable, this file does        *
 *  not by itself cause the resulting executable to be covered by the        *
 *  GNU Lesser General Public License. This exception does not however       *
 *  invalidate any other reasons why the executable file might be            *
 *  covered by the GNU Lesser General Public License.                        *
 *                                                                           *
 *  OpenMesh is distributed in the hope that it will be useful,              *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU Lesser General Public License for more details.                      *
 *                                                                           *
 *  You should have received a copy of the GNU LesserGeneral Public          *
 *  License along with OpenMesh.  If not,                                    *
 *  see <http://www.gnu.org/licenses/>.                                      *
 *                                                                           *
\*===========================================================================*/

/*===========================================================================*\
 *                                                                           *
 *   $Revision: 551 $                                                         *
 *   $Date: 2012-03-02 22:46:15 +0800 (周五, 02 三月 2012) $                   *
 *                                                                           *
\*===========================================================================*/


#ifndef OPENMESH_IO_OMFORMAT_HH
#define OPENMESH_IO_OMFORMAT_HH


//=== INCLUDES ================================================================

#include <OpenMesh/Core/System/config.h>
#include <OpenMesh/Core/System/omstream.hh>
#include <OpenMesh/Core/IO/SR_store.hh>
#include <OpenMesh/Core/Utils/GenProg.hh>
#include <OpenMesh/Core/Utils/Endian.hh>
#include <OpenMesh/Core/Utils/vector_traits.hh>
// --------------------
#include <iostream>
#if defined(OM_CC_GCC) && (OM_GCC_VERSION < 30000)
#  include <OpenMesh/Tools/Utils/NumLimitsT.hh>
#  define OM_MISSING_HEADER_LIMITS 1
#else
#  include <limits>
#endif


//== NAMESPACES ==============================================================

#ifndef DOXY_IGNORE_THIS
namespace OpenMesh {
namespace IO   {
namespace OMFormat {


//=== IMPLEMENTATION ==========================================================


/** \name Mesh Reading / Writing
*/
//@{

//-----------------------------------------------------------------------------

  // <:Header>
  // <:Comment>
  // Chunk 0
  //   <:ChunkHeader>
  //   <:Comment>
  //   data
  // Chunk 1
  //   <:ChunkHeader>
  //   <:Comment>
  //   data
  // .
  // .
  // .
  // Chunk N

  //
  // NOTICE!
  //
  // The usage of data types who differ in size
  // on different pc architectures (32/64 bit) and/or
  // operating systems, e.g. (unsigned) long, size_t,
  // is not recommended because of inconsistencies
  // in case of cross writing and reading.
  //
  // Basic types that are supported are:


  typedef unsigned char      uchar;
  typedef uint8_t            uint8;
  typedef uint16_t           uint16;
  typedef uint32_t           uint32;
  typedef uint64_t           uint64;
  typedef int8_t             int8;
  typedef int16_t            int16;
  typedef int32_t            int32;
  typedef int64_t            int64;
  typedef float32_t          float32;
  typedef float64_t          float64;

  struct Header
  {
    uchar  magic_[2]; // OM
    uchar  mesh_;    // [T]riangles, [Q]uads, [P]olygonals
    uint8  version_;
    uint32 n_vertices_;
    uint32 n_faces_;
    uint32 n_edges_;

    size_t store( std::ostream& _os, bool _swap ) const
    {
      _os.write( (char*)this, 4); // magic_, mesh_, version_
      size_t bytes = 4;
      bytes += binary<uint32_t>::store( _os, n_vertices_, _swap );
      bytes += binary<uint32_t>::store( _os, n_faces_, _swap );
      bytes += binary<uint32_t>::store( _os, n_edges_, _swap );
      return bytes;
    }

    size_t restore( std::istream& _is, bool _swap )
    {
      if (_is.read( (char*)this, 4 ).eof())
	return 0;

      size_t bytes = 4;
      bytes += binary<uint32_t>::restore( _is, n_vertices_, _swap );
      bytes += binary<uint32_t>::restore( _is, n_faces_, _swap );
      bytes += binary<uint32_t>::restore( _is, n_edges_, _swap );
      return bytes;
    }

  };

  struct Chunk
  {
    // Hardcoded this size to an uint32 to make the system 32/64 bit compatible.
    // Needs further investigation!
    typedef uint32 esize_t; // element size, used for custom properties

    enum Type {
      Type_Pos       = 0x00,
      Type_Normal    = 0x01,
      Type_Texcoord  = 0x02,
      Type_Status    = 0x03,
      Type_Color     = 0x04,
      Type_Custom    = 0x06,
      Type_Topology  = 0x07
    };

    enum Entity {
      Entity_Vertex    = 0x00,
      Entity_Mesh      = 0x01,
      Entity_Face      = 0x02,
      Entity_Edge      = 0x04,
      Entity_Halfedge  = 0x06
    };

    enum Dim {
      Dim_1D = 0x00,
      Dim_2D = 0x01,
      Dim_3D = 0x02,
      Dim_4D = 0x03,
      Dim_5D = 0x04,
      Dim_6D = 0x05,
      Dim_7D = 0x06,
      Dim_8D = 0x07
    };

    enum Integer_Size {
      Integer_8   = 0x00, // 1 byte for (unsigned) char
      Integer_16  = 0x01, // 2 bytes for short
      Integer_32  = 0x02, // 4 bytes for long
      Integer_64  = 0x03  // 8 bytes for long long
    };

    enum Float_Size {
      Float_32  = 0x00, //  4 bytes for float
      Float_64  = 0x01, //  8 bytes for double
      Float_128 = 0x02  // 16 bytes for long double (an assumption!)
    };

    static const int SIZE_RESERVED = 1; //  1
    static const int SIZE_NAME     = 1; //  2
    static const int SIZE_ENTITY   = 3; //  5
    static const int SIZE_TYPE     = 4; //  9

    static const int SIZE_SIGNED   = 1; // 10
    static const int SIZE_FLOAT    = 1; // 11
    static const int SIZE_DIM      = 3; // 14
    static const int SIZE_BITS     = 2; // 16

    static const int OFF_RESERVED = 0;                          //  0
    static const int OFF_NAME     = SIZE_RESERVED + OFF_RESERVED; //  2
    static const int OFF_ENTITY   = SIZE_NAME     + OFF_NAME;   //  3
    static const int OFF_TYPE     = SIZE_ENTITY   + OFF_ENTITY; //  5
    static const int OFF_SIGNED   = SIZE_TYPE     + OFF_TYPE;   //  9
    static const int OFF_FLOAT    = SIZE_SIGNED   + OFF_SIGNED; // 10
    static const int OFF_DIM      = SIZE_FLOAT    + OFF_FLOAT;  // 11
    static const int OFF_BITS     = SIZE_DIM      + OFF_DIM;    // 14

    // !Attention! When changing the bit size, the operators
    // << (uint16, Header) and << (Header, uint16) must be changed as well
    //
    // Entries signed_, float_, dim_, bits_ are not used when type_
    // equals Type_Custom
    //
    struct Header // 16 bits long
    {
      unsigned reserved_: SIZE_RESERVED;
      unsigned name_    : SIZE_NAME;   // 1 named property, 0 anonymous
      unsigned entity_  : SIZE_ENTITY; // 0 vertex, 1 mesh, 2 edge,
                                       // 4 halfedge, 6 face
      unsigned type_    : SIZE_TYPE;   // 0 pos, 1 normal, 2 texcoord,
                                       // 3 status, 4 color 6 custom 7 topology
      unsigned signed_  : SIZE_SIGNED; // bool
      unsigned float_   : SIZE_FLOAT;  // bool
      unsigned dim_     : SIZE_DIM;    // 0 1D, 1 2D, 2 3D, .., 7 8D
      unsigned bits_    : SIZE_BITS;   // {8, 16, 32, 64} | {32, 64, 128}
                                       // (integer)         (float)
      unsigned unused_  : 16; // fill up to 32 bits
    }; // struct Header


    class PropertyName : public std::string
    {
    public:

      static const size_t size_max = 256;

      PropertyName( ) { }

      PropertyName( const std::string& _name ) { *this = _name; }

      bool is_valid() const { return is_valid( size() ); }

      static bool is_valid( size_t _s ) { return _s <= size_max; }

      PropertyName& operator = ( const std::string& _rhs )
      {
	assert( is_valid( _rhs.size() ) );

	if ( is_valid( _rhs.size() ) )
	  std::string::operator = ( _rhs );
	else
	{
	  omerr() << "Warning! Property name too long. Will be shortened!\n";
	  this->std::string::operator = ( _rhs.substr(0, size_max) );
	}

	return *this;
      }

    };

  }; // Chunk

  // ------------------------------------------------------------ Helper

  // -------------------- get size information

  /// Return size of header in bytes.
  inline size_t header_size(void) { return sizeof(Header); }


  /// Return size of chunk header in bytes.
  inline size_t chunk_header_size( void ) { return sizeof(uint16); }


  /// Return the size of a scale in bytes.
  inline size_t scalar_size( const Chunk::Header& _hdr )
  {
    return _hdr.float_ ? (0x01 << _hdr.bits_) : (0x04 << _hdr.bits_);
  }


  /// Return the dimension of the vector in a chunk
  inline size_t dimensions(const Chunk::Header& _chdr) { return _chdr.dim_+1; }


  /// Return the size of a vector in bytes.
  inline size_t vector_size( const Chunk::Header& _chdr )
  {
    return dimensions(_chdr)*scalar_size(_chdr);
  }


  /// Return the size of chunk data in bytes
  inline size_t chunk_data_size( Header& _hdr,  Chunk::Header& _chunk_hdr )
  {
    size_t C     = 0;

    switch( _chunk_hdr.entity_ )
    {
      case Chunk::Entity_Vertex:   C  = _hdr.n_vertices_; break;
      case Chunk::Entity_Face:     C  = _hdr.n_faces_;    break;
      case Chunk::Entity_Halfedge: C  = _hdr.n_edges_;    // no break!
      case Chunk::Entity_Edge:     C += _hdr.n_edges_;    break;
      case Chunk::Entity_Mesh:     C  = 1;                break;
      default:
	std::cerr << "Invalid value in _chunk_hdr.entity_\n";
	assert( false );
    }

    return C * vector_size( _chunk_hdr );
  }

  inline size_t chunk_size( Header& _hdr, Chunk::Header& _chunk_hdr )
  {
    return chunk_header_size() + chunk_data_size( _hdr, _chunk_hdr );
  }

  // -------------------- convert from Chunk::Header to storage type

  uint16& operator << (uint16& val, const Chunk::Header& hdr);
  Chunk::Header& operator << (Chunk::Header& hdr, const uint16 val);


  // -------------------- type information

  template <typename T> bool is_float(const T&)
  {
#if defined(OM_MISSING_HEADER_LIMITS)
    return !Utils::NumLimitsT<T>::is_integer();
#else
    return !std::numeric_limits<T>::is_integer;
#endif
  }

  template <typename T> bool is_integer(const T)
  {
#if defined(OM_MISSING_HEADER_LIMITS)
    return Utils::NumLimitsT<T>::is_integer();
#else
    return std::numeric_limits<T>::is_integer;
#endif
  }

  template <typename T> bool is_signed(const T&)
  {
#if defined(OM_MISSING_HEADER_LIMITS)
    return Utils::NumLimitsT<T>::is_signed();
#else
    return std::numeric_limits<T>::is_signed;
#endif
  }

  // -------------------- conversions (format type <- type/value)

  template <typename VecType>
  inline
  Chunk::Dim dim( VecType )
  {
    assert( vector_traits< VecType >::size() < 9 );
    return static_cast<Chunk::Dim>(vector_traits< VecType >::size() - 1);
  }

  template <typename VecType>
  inline
  Chunk::Dim dim( const Chunk::Header& _hdr )
  {
    return static_cast<Chunk::Dim>( _hdr.dim_ );
  }

  // calc minimum (power-of-2) number of bits needed
  Chunk::Integer_Size needed_bits( size_t s );

  // Convert size of type to Integer_Size
#ifdef NDEBUG
  template <typename T> Chunk::Integer_Size integer_size(const T&)
#else
  template <typename T> Chunk::Integer_Size integer_size(const T& d)
#endif
  {
    assert( is_integer(d) );

    switch( sizeof(T) )
    {
      case  1: return OMFormat::Chunk::Integer_8;
      case  2: return OMFormat::Chunk::Integer_16;
      case  4: return OMFormat::Chunk::Integer_32;
      case  8: return OMFormat::Chunk::Integer_64;
    }
    return Chunk::Integer_Size(0);
  }


  // Convert size of type to FLoat_Size
#ifdef NDEBUG
  template <typename T> Chunk::Float_Size float_size(const T&)
#else
  template <typename T> Chunk::Float_Size float_size(const T& d)
#endif
  {
    assert( is_float(d) );

    switch( sizeof(T) )
    {
      case  4: return OMFormat::Chunk::Float_32;
      case  8: return OMFormat::Chunk::Float_64;
      case 16: return OMFormat::Chunk::Float_128;
    }
    return Chunk::Float_Size(0);
  }

  // Return the storage type (Chunk::Header::bits_)
  template <typename T>
  inline
  unsigned int bits(const T& val)
  {
    return is_integer(val)
      ? (static_cast<unsigned int>(integer_size(val)))
      : (static_cast<unsigned int>(float_size(val)));
  }

  // -------------------- create/read version

  inline uint8 mk_version(const uint16 major, const uint16 minor)
  { return (major & 0x07) << 5 | (minor & 0x1f); }


  inline uint16 major_version(const uint8 version)
  { return (version >> 5) & 0x07; }


  inline uint16 minor_version(const uint8 version)
  { return (version & 0x001f); }


  // ---------------------------------------- convenience functions

  const char *as_string(Chunk::Type t);
  const char *as_string(Chunk::Entity e);
  const char *as_string(Chunk::Dim d);
  const char *as_string(Chunk::Integer_Size d);
  const char *as_string(Chunk::Float_Size d);

  std::ostream& operator << ( std::ostream& _os, const Header& _h );
  std::ostream& operator << ( std::ostream& _os, const Chunk::Header& _c );

//@}
} // namespace OMFormat

  // -------------------- (re-)store header

  template <> inline
  size_t store( std::ostream& _os, const OMFormat::Header& _hdr, bool _swap)
  { return _hdr.store( _os, _swap ); }

  template <> inline
  size_t restore( std::istream& _is, OMFormat::Header& _hdr, bool _swap )
  { return _hdr.restore( _is, _swap ); }


  // -------------------- (re-)store chunk header

  template <> inline
  size_t
  store( std::ostream& _os, const OMFormat::Chunk::Header& _hdr, bool _swap)
  {
    OMFormat::uint16 val; val << _hdr;
    return binary<uint16_t>::store( _os, val, _swap );
  }

  template <> inline
  size_t
  restore( std::istream& _is, OMFormat::Chunk::Header& _hdr, bool _swap )
  {
    OMFormat::uint16 val;
    size_t bytes = binary<uint16_t>::restore( _is, val, _swap );

    _hdr << val;

    return bytes;
  }

  // -------------------- (re-)store integer with wanted number of bits (bytes)

  typedef GenProg::True  t_signed;
  typedef GenProg::False t_unsigned;

  // helper to store a an integer
  template< typename T >
  size_t
  store( std::ostream& _os,
	 const T& _val,
	 OMFormat::Chunk::Integer_Size _b,
	 bool _swap,
	 t_signed);

  // helper to store a an unsigned integer
  template< typename T >
  size_t
  store( std::ostream& _os,
	 const T& _val,
	 OMFormat::Chunk::Integer_Size _b,
	 bool _swap,
	 t_unsigned);

  /// Store an integer with a wanted number of bits
  template< typename T >
  inline
  size_t
  store( std::ostream& _os,
	 const T& _val,
	 OMFormat::Chunk::Integer_Size _b,
	 bool _swap)
  {
    assert( OMFormat::is_integer( _val ) );

    if ( OMFormat::is_signed( _val ) )
      return store( _os, _val, _b, _swap, t_signed()   );
    return   store( _os, _val, _b, _swap, t_unsigned() );
  }

  // helper to store a an integer
  template< typename T > inline
  size_t restore( std::istream& _is,
		  T& _val,
		  OMFormat::Chunk::Integer_Size _b,
		  bool _swap,
		  t_signed);

  // helper to store a an unsigned integer
  template< typename T > inline
  size_t restore( std::istream& _is,
		  T& _val,
		  OMFormat::Chunk::Integer_Size _b,
		  bool _swap,
		  t_unsigned);

  /// Restore an integer with a wanted number of bits
  template< typename T >
  inline
  size_t
  restore( std::istream& _is,
	   T& _val,
	   OMFormat::Chunk::Integer_Size _b,
	   bool _swap)
  {
    assert( OMFormat::is_integer( _val ) );

    if ( OMFormat::is_signed( _val ) )
      return restore( _is, _val, _b, _swap, t_signed() );
    return restore( _is, _val, _b, _swap, t_unsigned() );
  }


  //
  // ---------------------------------------- storing vectors
  template <typename VecT> inline
  size_t store( std::ostream& _os, const VecT& _vec, GenProg::Int2Type<2>,
		bool _swap )
  {
    size_t bytes =  store( _os, _vec[0], _swap );
    bytes += store( _os, _vec[1], _swap );
    return bytes;
  }

  template <typename VecT> inline
  size_t store( std::ostream& _os, const VecT& _vec, GenProg::Int2Type<3>,
		bool _swap )
  {
    size_t bytes =  store( _os, _vec[0], _swap );
    bytes += store( _os, _vec[1], _swap );
    bytes += store( _os, _vec[2], _swap );
    return bytes;
  }

  template <typename VecT> inline
  size_t store( std::ostream& _os, const VecT& _vec, GenProg::Int2Type<4>,
		bool _swap )
  {
    size_t bytes =  store( _os, _vec[0], _swap );
    bytes += store( _os, _vec[1], _swap );
    bytes += store( _os, _vec[2], _swap );
    bytes += store( _os, _vec[3], _swap );
    return bytes;
  }

  template <typename VecT> inline
  size_t store( std::ostream& _os, const VecT& _vec, GenProg::Int2Type<1>,
		bool _swap )
  {
    return store( _os, _vec[0], _swap );
  }

  /// storing a vector type
  template <typename VecT> inline
  size_t vector_store( std::ostream& _os, const VecT& _vec, bool _swap )
  {
    return store( _os, _vec,
		  GenProg::Int2Type< vector_traits<VecT>::size_ >(),
		  _swap );
  }

  // ---------------------------------------- restoring vectors
  template <typename VecT>
  inline
  size_t
  restore( std::istream& _is, VecT& _vec, GenProg::Int2Type<2>,
	 bool _swap )
  {
    size_t bytes =  restore( _is, _vec[0], _swap );
    bytes += restore( _is, _vec[1], _swap );
    return bytes;
  }

  template <typename VecT>
  inline
  size_t
  restore( std::istream& _is, VecT& _vec, GenProg::Int2Type<3>,
	 bool _swap )
  {
    typedef typename vector_traits<VecT>::value_type scalar_type;
    size_t bytes;

    bytes  = binary<scalar_type>::restore( _is, _vec[0], _swap );
    bytes += binary<scalar_type>::restore( _is, _vec[1], _swap );
    bytes += binary<scalar_type>::restore( _is, _vec[2], _swap );
    return bytes;
  }

  template <typename VecT>
  inline
  size_t
  restore( std::istream& _is, VecT& _vec, GenProg::Int2Type<4>,
	   bool _swap )
  {
    typedef typename vector_traits<VecT>::value_type scalar_type;
    size_t bytes;

    bytes  = binary<scalar_type>::restore( _is, _vec[0], _swap );
    bytes += binary<scalar_type>::restore( _is, _vec[1], _swap );
    bytes += binary<scalar_type>::restore( _is, _vec[2], _swap );
    bytes += binary<scalar_type>::restore( _is, _vec[3], _swap );
    return bytes;
  }

  template <typename VecT>
  inline
  size_t
  restore( std::istream& _is, VecT& _vec, GenProg::Int2Type<1>,
	 bool _swap )
  {
    return restore( _is, _vec[0], _swap );
  }

  /// Restoring a vector type
  template <typename VecT>
  inline
  size_t
  vector_restore( std::istream& _is, VecT& _vec, bool _swap )
  {
    return restore( _is, _vec,
		    GenProg::Int2Type< vector_traits<VecT>::size_ >(),
		    _swap );
  }


  // ---------------------------------------- storing property names

  template <>
  inline
  size_t store( std::ostream& _os, const OMFormat::Chunk::PropertyName& _pn,
		bool _swap )
  {
    store( _os, _pn.size(), OMFormat::Chunk::Integer_8, _swap ); // 1 byte
    if ( _pn.size() )
      _os.write( _pn.c_str(), _pn.size() ); // size bytes
    return _pn.size() + 1;
  }

  template <>
  inline
  size_t restore( std::istream& _is, OMFormat::Chunk::PropertyName& _pn,
		  bool _swap )
  {
    size_t size;

    restore( _is, size, OMFormat::Chunk::Integer_8, _swap); // 1 byte

    assert( OMFormat::Chunk::PropertyName::is_valid( size ) );

    if ( size > 0 )
    {
      char buf[256];
      _is.read( buf, size ); // size bytes
      buf[size] = '\0';
      _pn.resize(size);
      _pn = buf;
    }
    return size+1;
  }

//=============================================================================
} // namespace IO
} // namespace OpenMesh
#endif
//=============================================================================
#if defined(OM_MISSING_HEADER_LIMITS)
#  undef OM_MISSING_HEADER_LIMITS
#endif
//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(OPENMESH_IO_OMFORMAT_CC)
#  define OPENMESH_IO_OMFORMAT_TEMPLATES
#  include "OMFormatT.cc"
#endif
//=============================================================================
#endif
//=============================================================================
