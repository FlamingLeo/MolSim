// Copyright (c) 2005-2023 Code Synthesis.
//
// This program was generated by CodeSynthesis XSD, an XML Schema to
// C++ data binding compiler.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
// In addition, as a special exception, Code Synthesis gives permission
// to link this program with the Xerces-C++ library (or with modified
// versions of Xerces-C++ that use the same license as Xerces-C++), and
// distribute linked combinations including the two. You must obey the GNU
// General Public License version 2 in all respects for all of the code
// used other than Xerces-C++. If you modify this copy of the program, you
// may extend this exception to your version of the program, but you are
// not obligated to do so. If you do not wish to do so, delete this
// exception statement from your version.
//
// Furthermore, Code Synthesis makes a special exception for the Free/Libre
// and Open Source Software (FLOSS) which is described in the accompanying
// FLOSSE file.
//

#ifndef SIMULATION_XSD_H
#define SIMULATION_XSD_H

#ifndef XSD_CXX11
#define XSD_CXX11
#endif

#ifndef XSD_USE_CHAR
#define XSD_USE_CHAR
#endif

#ifndef XSD_CXX_TREE_USE_CHAR
#define XSD_CXX_TREE_USE_CHAR
#endif

// Begin prologue.
//
//
// End prologue.

#include <xsd/cxx/config.hxx>

#include <xsd/cxx/pre.hxx>

#include <xsd/cxx/xml/char-utf8.hxx>

#include <xsd/cxx/tree/elements.hxx>
#include <xsd/cxx/tree/exceptions.hxx>
#include <xsd/cxx/tree/types.hxx>

#include <xsd/cxx/xml/error-handler.hxx>

#include <xsd/cxx/xml/dom/auto-ptr.hxx>

#include <xsd/cxx/tree/parsing.hxx>
#include <xsd/cxx/tree/parsing/boolean.hxx>
#include <xsd/cxx/tree/parsing/byte.hxx>
#include <xsd/cxx/tree/parsing/decimal.hxx>
#include <xsd/cxx/tree/parsing/double.hxx>
#include <xsd/cxx/tree/parsing/float.hxx>
#include <xsd/cxx/tree/parsing/int.hxx>
#include <xsd/cxx/tree/parsing/long.hxx>
#include <xsd/cxx/tree/parsing/short.hxx>
#include <xsd/cxx/tree/parsing/unsigned-byte.hxx>
#include <xsd/cxx/tree/parsing/unsigned-int.hxx>
#include <xsd/cxx/tree/parsing/unsigned-long.hxx>
#include <xsd/cxx/tree/parsing/unsigned-short.hxx>

#include <xsd/cxx/tree/serialization.hxx>
#include <xsd/cxx/tree/serialization/boolean.hxx>
#include <xsd/cxx/tree/serialization/byte.hxx>
#include <xsd/cxx/tree/serialization/decimal.hxx>
#include <xsd/cxx/tree/serialization/double.hxx>
#include <xsd/cxx/tree/serialization/float.hxx>
#include <xsd/cxx/tree/serialization/int.hxx>
#include <xsd/cxx/tree/serialization/long.hxx>
#include <xsd/cxx/tree/serialization/short.hxx>
#include <xsd/cxx/tree/serialization/unsigned-byte.hxx>
#include <xsd/cxx/tree/serialization/unsigned-int.hxx>
#include <xsd/cxx/tree/serialization/unsigned-long.hxx>
#include <xsd/cxx/tree/serialization/unsigned-short.hxx>
#include <xsd/cxx/xml/dom/serialization-header.hxx>

namespace xml_schema {
// anyType and anySimpleType.
//
typedef ::xsd::cxx::tree::type Type;
typedef ::xsd::cxx::tree::simple_type<char, Type> SimpleType;
typedef ::xsd::cxx::tree::type Container;

// 8-bit
//
typedef signed char Byte;
typedef unsigned char UnsignedByte;

// 16-bit
//
typedef short Short;
typedef unsigned short UnsignedShort;

// 32-bit
//
typedef int Int;
typedef unsigned int UnsignedInt;

// 64-bit
//
typedef long long Long;
typedef unsigned long long UnsignedLong;

// Supposed to be arbitrary-length integral types.
//
typedef long long Integer;
typedef long long NonPositiveInteger;
typedef unsigned long long NonNegativeInteger;
typedef unsigned long long PositiveInteger;
typedef long long NegativeInteger;

// Boolean.
//
typedef bool Boolean;

// Floating-point types.
//
typedef float Float;
typedef double Double;
typedef double Decimal;

// String types.
//
typedef ::xsd::cxx::tree::string<char, SimpleType> String;
typedef ::xsd::cxx::tree::normalized_string<char, String> NormalizedString;
typedef ::xsd::cxx::tree::token<char, NormalizedString> Token;
typedef ::xsd::cxx::tree::name<char, Token> Name;
typedef ::xsd::cxx::tree::nmtoken<char, Token> Nmtoken;
typedef ::xsd::cxx::tree::nmtokens<char, SimpleType, Nmtoken> Nmtokens;
typedef ::xsd::cxx::tree::ncname<char, Name> Ncname;
typedef ::xsd::cxx::tree::language<char, Token> Language;

// ID/IDREF.
//
typedef ::xsd::cxx::tree::id<char, Ncname> Id;
typedef ::xsd::cxx::tree::idref<char, Ncname, Type> Idref;
typedef ::xsd::cxx::tree::idrefs<char, SimpleType, Idref> Idrefs;

// URI.
//
typedef ::xsd::cxx::tree::uri<char, SimpleType> Uri;

// Qualified name.
//
typedef ::xsd::cxx::tree::qname<char, SimpleType, Uri, Ncname> Qname;

// Binary.
//
typedef ::xsd::cxx::tree::buffer<char> Buffer;
typedef ::xsd::cxx::tree::base64_binary<char, SimpleType> Base64Binary;
typedef ::xsd::cxx::tree::hex_binary<char, SimpleType> HexBinary;

// Date/time.
//
typedef ::xsd::cxx::tree::time_zone TimeZone;
typedef ::xsd::cxx::tree::date<char, SimpleType> Date;
typedef ::xsd::cxx::tree::date_time<char, SimpleType> DateTime;
typedef ::xsd::cxx::tree::duration<char, SimpleType> Duration;
typedef ::xsd::cxx::tree::gday<char, SimpleType> Gday;
typedef ::xsd::cxx::tree::gmonth<char, SimpleType> Gmonth;
typedef ::xsd::cxx::tree::gmonth_day<char, SimpleType> GmonthDay;
typedef ::xsd::cxx::tree::gyear<char, SimpleType> Gyear;
typedef ::xsd::cxx::tree::gyear_month<char, SimpleType> GyearMonth;
typedef ::xsd::cxx::tree::time<char, SimpleType> Time;

// Entity.
//
typedef ::xsd::cxx::tree::entity<char, Ncname> Entity;
typedef ::xsd::cxx::tree::entities<char, SimpleType, Entity> Entities;

typedef ::xsd::cxx::tree::content_order ContentOrder;
// Namespace information and list stream. Used in
// serialization functions.
//
typedef ::xsd::cxx::xml::dom::namespace_info<char> NamespaceInfo;
typedef ::xsd::cxx::xml::dom::namespace_infomap<char> NamespaceInfomap;
typedef ::xsd::cxx::tree::list_stream<char> ListStream;
typedef ::xsd::cxx::tree::as_double<Double> AsDouble;
typedef ::xsd::cxx::tree::as_decimal<Decimal> AsDecimal;
typedef ::xsd::cxx::tree::facet Facet;

// Flags and properties.
//
typedef ::xsd::cxx::tree::flags Flags;
typedef ::xsd::cxx::tree::properties<char> Properties;

// Parsing/serialization diagnostics.
//
typedef ::xsd::cxx::tree::severity Severity;
typedef ::xsd::cxx::tree::error<char> Error;
typedef ::xsd::cxx::tree::diagnostics<char> Diagnostics;

// Exceptions.
//
typedef ::xsd::cxx::tree::exception<char> Exception;
typedef ::xsd::cxx::tree::bounds<char> Bounds;
typedef ::xsd::cxx::tree::duplicate_id<char> DuplicateId;
typedef ::xsd::cxx::tree::parsing<char> Parsing;
typedef ::xsd::cxx::tree::expected_element<char> ExpectedElement;
typedef ::xsd::cxx::tree::unexpected_element<char> UnexpectedElement;
typedef ::xsd::cxx::tree::expected_attribute<char> ExpectedAttribute;
typedef ::xsd::cxx::tree::unexpected_enumerator<char> UnexpectedEnumerator;
typedef ::xsd::cxx::tree::expected_text_content<char> ExpectedTextContent;
typedef ::xsd::cxx::tree::no_prefix_mapping<char> NoPrefixMapping;
typedef ::xsd::cxx::tree::serialization<char> Serialization;

// Error handler callback interface.
//
typedef ::xsd::cxx::xml::error_handler<char> ErrorHandler;

// DOM interaction.
//
namespace dom {
// Automatic pointer for DOMDocument.
//
using ::xsd::cxx::xml::dom::unique_ptr;

#ifndef XSD_CXX_TREE_TREE_NODE_KEY__XML_SCHEMA
#define XSD_CXX_TREE_TREE_NODE_KEY__XML_SCHEMA
// DOM user data key for back pointers to tree nodes.
//
const XMLCh *const treeNodeKey = ::xsd::cxx::tree::user_data_keys::node;
#endif
} // namespace dom
} // namespace xml_schema

// Forward declarations.
//
class ArgsType;
class PositionType;
class VelocityType;
class SizeType;
class CuboidType;
class ObjectsType;
class SimType;

#include <algorithm> // std::binary_search
#include <limits>    // std::numeric_limits
#include <memory>    // ::std::unique_ptr
#include <utility>   // std::move

#include <xsd/cxx/xml/char-utf8.hxx>

#include <xsd/cxx/tree/containers.hxx>
#include <xsd/cxx/tree/elements.hxx>
#include <xsd/cxx/tree/exceptions.hxx>
#include <xsd/cxx/tree/list.hxx>

#include <xsd/cxx/xml/dom/parsing-header.hxx>

class ArgsType : public ::xml_schema::Type {
  public:
    // startTime
    //
    typedef ::xml_schema::Double StartTimeType;
    typedef ::xsd::cxx::tree::optional<StartTimeType> StartTimeOptional;
    typedef ::xsd::cxx::tree::traits<StartTimeType, char, ::xsd::cxx::tree::schema_type::double_> StartTimeTraits;

    const StartTimeOptional &startTime() const;

    StartTimeOptional &startTime();

    void startTime(const StartTimeType &x);

    void startTime(const StartTimeOptional &x);

    // endTime
    //
    typedef ::xml_schema::Double EndTimeType;
    typedef ::xsd::cxx::tree::optional<EndTimeType> EndTimeOptional;
    typedef ::xsd::cxx::tree::traits<EndTimeType, char, ::xsd::cxx::tree::schema_type::double_> EndTimeTraits;

    const EndTimeOptional &endTime() const;

    EndTimeOptional &endTime();

    void endTime(const EndTimeType &x);

    void endTime(const EndTimeOptional &x);

    // delta_t
    //
    typedef ::xml_schema::Double Delta_tType;
    typedef ::xsd::cxx::tree::optional<Delta_tType> Delta_tOptional;
    typedef ::xsd::cxx::tree::traits<Delta_tType, char, ::xsd::cxx::tree::schema_type::double_> Delta_tTraits;

    const Delta_tOptional &delta_t() const;

    Delta_tOptional &delta_t();

    void delta_t(const Delta_tType &x);

    void delta_t(const Delta_tOptional &x);

    // epsilon
    //
    typedef ::xml_schema::Double EpsilonType;
    typedef ::xsd::cxx::tree::optional<EpsilonType> EpsilonOptional;
    typedef ::xsd::cxx::tree::traits<EpsilonType, char, ::xsd::cxx::tree::schema_type::double_> EpsilonTraits;

    const EpsilonOptional &epsilon() const;

    EpsilonOptional &epsilon();

    void epsilon(const EpsilonType &x);

    void epsilon(const EpsilonOptional &x);

    // sigma
    //
    typedef ::xml_schema::Double SigmaType;
    typedef ::xsd::cxx::tree::optional<SigmaType> SigmaOptional;
    typedef ::xsd::cxx::tree::traits<SigmaType, char, ::xsd::cxx::tree::schema_type::double_> SigmaTraits;

    const SigmaOptional &sigma() const;

    SigmaOptional &sigma();

    void sigma(const SigmaType &x);

    void sigma(const SigmaOptional &x);

    // frequency
    //
    typedef ::xml_schema::Int FrequencyType;
    typedef ::xsd::cxx::tree::optional<FrequencyType> FrequencyOptional;
    typedef ::xsd::cxx::tree::traits<FrequencyType, char> FrequencyTraits;

    const FrequencyOptional &frequency() const;

    FrequencyOptional &frequency();

    void frequency(const FrequencyType &x);

    void frequency(const FrequencyOptional &x);

    // basename
    //
    typedef ::xml_schema::String BasenameType;
    typedef ::xsd::cxx::tree::optional<BasenameType> BasenameOptional;
    typedef ::xsd::cxx::tree::traits<BasenameType, char> BasenameTraits;

    const BasenameOptional &basename() const;

    BasenameOptional &basename();

    void basename(const BasenameType &x);

    void basename(const BasenameOptional &x);

    void basename(::std::unique_ptr<BasenameType> p);

    // output
    //
    typedef ::xml_schema::String OutputType;
    typedef ::xsd::cxx::tree::optional<OutputType> OutputOptional;
    typedef ::xsd::cxx::tree::traits<OutputType, char> OutputTraits;

    const OutputOptional &output() const;

    OutputOptional &output();

    void output(const OutputType &x);

    void output(const OutputOptional &x);

    void output(::std::unique_ptr<OutputType> p);

    // simType
    //
    typedef ::xml_schema::String SimTypeType;
    typedef ::xsd::cxx::tree::optional<SimTypeType> SimTypeOptional;
    typedef ::xsd::cxx::tree::traits<SimTypeType, char> SimTypeTraits;

    const SimTypeOptional &simType() const;

    SimTypeOptional &simType();

    void simType(const SimTypeType &x);

    void simType(const SimTypeOptional &x);

    void simType(::std::unique_ptr<SimTypeType> p);

    // Constructors.
    //
    ArgsType();

    ArgsType(const ::xercesc::DOMElement &e, ::xml_schema::Flags f = 0, ::xml_schema::Container *c = 0);

    ArgsType(const ArgsType &x, ::xml_schema::Flags f = 0, ::xml_schema::Container *c = 0);

    virtual ArgsType *_clone(::xml_schema::Flags f = 0, ::xml_schema::Container *c = 0) const;

    ArgsType &operator=(const ArgsType &x);

    virtual ~ArgsType();

    // Implementation.
    //
  protected:
    void parse(::xsd::cxx::xml::dom::parser<char> &, ::xml_schema::Flags);

  protected:
    StartTimeOptional startTime_;
    EndTimeOptional endTime_;
    Delta_tOptional delta_t_;
    EpsilonOptional epsilon_;
    SigmaOptional sigma_;
    FrequencyOptional frequency_;
    BasenameOptional basename_;
    OutputOptional output_;
    SimTypeOptional simType_;
};

class PositionType : public ::xml_schema::Type {
  public:
    // x
    //
    typedef ::xml_schema::Double XType;
    typedef ::xsd::cxx::tree::traits<XType, char, ::xsd::cxx::tree::schema_type::double_> XTraits;

    const XType &x() const;

    XType &x();

    void x(const XType &x);

    // y
    //
    typedef ::xml_schema::Double YType;
    typedef ::xsd::cxx::tree::traits<YType, char, ::xsd::cxx::tree::schema_type::double_> YTraits;

    const YType &y() const;

    YType &y();

    void y(const YType &x);

    // z
    //
    typedef ::xml_schema::Double ZType;
    typedef ::xsd::cxx::tree::traits<ZType, char, ::xsd::cxx::tree::schema_type::double_> ZTraits;

    const ZType &z() const;

    ZType &z();

    void z(const ZType &x);

    // Constructors.
    //
    PositionType(const XType &, const YType &, const ZType &);

    PositionType(const ::xercesc::DOMElement &e, ::xml_schema::Flags f = 0, ::xml_schema::Container *c = 0);

    PositionType(const PositionType &x, ::xml_schema::Flags f = 0, ::xml_schema::Container *c = 0);

    virtual PositionType *_clone(::xml_schema::Flags f = 0, ::xml_schema::Container *c = 0) const;

    PositionType &operator=(const PositionType &x);

    virtual ~PositionType();

    // Implementation.
    //
  protected:
    void parse(::xsd::cxx::xml::dom::parser<char> &, ::xml_schema::Flags);

  protected:
    ::xsd::cxx::tree::one<XType> x_;
    ::xsd::cxx::tree::one<YType> y_;
    ::xsd::cxx::tree::one<ZType> z_;
};

class VelocityType : public ::xml_schema::Type {
  public:
    // x
    //
    typedef ::xml_schema::Double XType;
    typedef ::xsd::cxx::tree::traits<XType, char, ::xsd::cxx::tree::schema_type::double_> XTraits;

    const XType &x() const;

    XType &x();

    void x(const XType &x);

    // y
    //
    typedef ::xml_schema::Double YType;
    typedef ::xsd::cxx::tree::traits<YType, char, ::xsd::cxx::tree::schema_type::double_> YTraits;

    const YType &y() const;

    YType &y();

    void y(const YType &x);

    // z
    //
    typedef ::xml_schema::Double ZType;
    typedef ::xsd::cxx::tree::traits<ZType, char, ::xsd::cxx::tree::schema_type::double_> ZTraits;

    const ZType &z() const;

    ZType &z();

    void z(const ZType &x);

    // Constructors.
    //
    VelocityType(const XType &, const YType &, const ZType &);

    VelocityType(const ::xercesc::DOMElement &e, ::xml_schema::Flags f = 0, ::xml_schema::Container *c = 0);

    VelocityType(const VelocityType &x, ::xml_schema::Flags f = 0, ::xml_schema::Container *c = 0);

    virtual VelocityType *_clone(::xml_schema::Flags f = 0, ::xml_schema::Container *c = 0) const;

    VelocityType &operator=(const VelocityType &x);

    virtual ~VelocityType();

    // Implementation.
    //
  protected:
    void parse(::xsd::cxx::xml::dom::parser<char> &, ::xml_schema::Flags);

  protected:
    ::xsd::cxx::tree::one<XType> x_;
    ::xsd::cxx::tree::one<YType> y_;
    ::xsd::cxx::tree::one<ZType> z_;
};

class SizeType : public ::xml_schema::Type {
  public:
    // x
    //
    typedef ::xml_schema::Long XType;
    typedef ::xsd::cxx::tree::traits<XType, char> XTraits;

    const XType &x() const;

    XType &x();

    void x(const XType &x);

    // y
    //
    typedef ::xml_schema::Long YType;
    typedef ::xsd::cxx::tree::traits<YType, char> YTraits;

    const YType &y() const;

    YType &y();

    void y(const YType &x);

    // z
    //
    typedef ::xml_schema::Long ZType;
    typedef ::xsd::cxx::tree::traits<ZType, char> ZTraits;

    const ZType &z() const;

    ZType &z();

    void z(const ZType &x);

    // Constructors.
    //
    SizeType(const XType &, const YType &, const ZType &);

    SizeType(const ::xercesc::DOMElement &e, ::xml_schema::Flags f = 0, ::xml_schema::Container *c = 0);

    SizeType(const SizeType &x, ::xml_schema::Flags f = 0, ::xml_schema::Container *c = 0);

    virtual SizeType *_clone(::xml_schema::Flags f = 0, ::xml_schema::Container *c = 0) const;

    SizeType &operator=(const SizeType &x);

    virtual ~SizeType();

    // Implementation.
    //
  protected:
    void parse(::xsd::cxx::xml::dom::parser<char> &, ::xml_schema::Flags);

  protected:
    ::xsd::cxx::tree::one<XType> x_;
    ::xsd::cxx::tree::one<YType> y_;
    ::xsd::cxx::tree::one<ZType> z_;
};

class CuboidType : public ::xml_schema::Type {
  public:
    // position
    //
    typedef ::PositionType PositionType;
    typedef ::xsd::cxx::tree::traits<PositionType, char> PositionTraits;

    const PositionType &position() const;

    PositionType &position();

    void position(const PositionType &x);

    void position(::std::unique_ptr<PositionType> p);

    // velocity
    //
    typedef ::VelocityType VelocityType;
    typedef ::xsd::cxx::tree::traits<VelocityType, char> VelocityTraits;

    const VelocityType &velocity() const;

    VelocityType &velocity();

    void velocity(const VelocityType &x);

    void velocity(::std::unique_ptr<VelocityType> p);

    // size
    //
    typedef ::SizeType SizeType;
    typedef ::xsd::cxx::tree::traits<SizeType, char> SizeTraits;

    const SizeType &size() const;

    SizeType &size();

    void size(const SizeType &x);

    void size(::std::unique_ptr<SizeType> p);

    // distance
    //
    typedef ::xml_schema::Double DistanceType;
    typedef ::xsd::cxx::tree::traits<DistanceType, char, ::xsd::cxx::tree::schema_type::double_> DistanceTraits;

    const DistanceType &distance() const;

    DistanceType &distance();

    void distance(const DistanceType &x);

    // mass
    //
    typedef ::xml_schema::Double MassType;
    typedef ::xsd::cxx::tree::traits<MassType, char, ::xsd::cxx::tree::schema_type::double_> MassTraits;

    const MassType &mass() const;

    MassType &mass();

    void mass(const MassType &x);

    // Constructors.
    //
    CuboidType(const PositionType &, const VelocityType &, const SizeType &, const DistanceType &, const MassType &);

    CuboidType(::std::unique_ptr<PositionType>, ::std::unique_ptr<VelocityType>, ::std::unique_ptr<SizeType>,
               const DistanceType &, const MassType &);

    CuboidType(const ::xercesc::DOMElement &e, ::xml_schema::Flags f = 0, ::xml_schema::Container *c = 0);

    CuboidType(const CuboidType &x, ::xml_schema::Flags f = 0, ::xml_schema::Container *c = 0);

    virtual CuboidType *_clone(::xml_schema::Flags f = 0, ::xml_schema::Container *c = 0) const;

    CuboidType &operator=(const CuboidType &x);

    virtual ~CuboidType();

    // Implementation.
    //
  protected:
    void parse(::xsd::cxx::xml::dom::parser<char> &, ::xml_schema::Flags);

  protected:
    ::xsd::cxx::tree::one<PositionType> position_;
    ::xsd::cxx::tree::one<VelocityType> velocity_;
    ::xsd::cxx::tree::one<SizeType> size_;
    ::xsd::cxx::tree::one<DistanceType> distance_;
    ::xsd::cxx::tree::one<MassType> mass_;
};

class ObjectsType : public ::xml_schema::Type {
  public:
    // cuboid
    //
    typedef ::CuboidType CuboidType;
    typedef ::xsd::cxx::tree::sequence<CuboidType> CuboidSequence;
    typedef CuboidSequence::iterator CuboidIterator;
    typedef CuboidSequence::const_iterator CuboidConstIterator;
    typedef ::xsd::cxx::tree::traits<CuboidType, char> CuboidTraits;

    const CuboidSequence &cuboid() const;

    CuboidSequence &cuboid();

    void cuboid(const CuboidSequence &s);

    // Constructors.
    //
    ObjectsType();

    ObjectsType(const ::xercesc::DOMElement &e, ::xml_schema::Flags f = 0, ::xml_schema::Container *c = 0);

    ObjectsType(const ObjectsType &x, ::xml_schema::Flags f = 0, ::xml_schema::Container *c = 0);

    virtual ObjectsType *_clone(::xml_schema::Flags f = 0, ::xml_schema::Container *c = 0) const;

    ObjectsType &operator=(const ObjectsType &x);

    virtual ~ObjectsType();

    // Implementation.
    //
  protected:
    void parse(::xsd::cxx::xml::dom::parser<char> &, ::xml_schema::Flags);

  protected:
    CuboidSequence cuboid_;
};

class SimType : public ::xml_schema::Type {
  public:
    // args
    //
    typedef ::ArgsType ArgsType;
    typedef ::xsd::cxx::tree::optional<ArgsType> ArgsOptional;
    typedef ::xsd::cxx::tree::traits<ArgsType, char> ArgsTraits;

    const ArgsOptional &args() const;

    ArgsOptional &args();

    void args(const ArgsType &x);

    void args(const ArgsOptional &x);

    void args(::std::unique_ptr<ArgsType> p);

    // type
    //
    typedef ::xml_schema::String TypeType;
    typedef ::xsd::cxx::tree::traits<TypeType, char> TypeTraits;

    const TypeType &type() const;

    TypeType &type();

    void type(const TypeType &x);

    void type(::std::unique_ptr<TypeType> p);

    // objects
    //
    typedef ::ObjectsType ObjectsType;
    typedef ::xsd::cxx::tree::traits<ObjectsType, char> ObjectsTraits;

    const ObjectsType &objects() const;

    ObjectsType &objects();

    void objects(const ObjectsType &x);

    void objects(::std::unique_ptr<ObjectsType> p);

    // Constructors.
    //
    SimType(const TypeType &, const ObjectsType &);

    SimType(const TypeType &, ::std::unique_ptr<ObjectsType>);

    SimType(const ::xercesc::DOMElement &e, ::xml_schema::Flags f = 0, ::xml_schema::Container *c = 0);

    SimType(const SimType &x, ::xml_schema::Flags f = 0, ::xml_schema::Container *c = 0);

    virtual SimType *_clone(::xml_schema::Flags f = 0, ::xml_schema::Container *c = 0) const;

    SimType &operator=(const SimType &x);

    virtual ~SimType();

    // Implementation.
    //
  protected:
    void parse(::xsd::cxx::xml::dom::parser<char> &, ::xml_schema::Flags);

  protected:
    ArgsOptional args_;
    ::xsd::cxx::tree::one<TypeType> type_;
    ::xsd::cxx::tree::one<ObjectsType> objects_;
};

#include <iosfwd>

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/sax/InputSource.hpp>

// Parse a URI or a local file.
//

::std::unique_ptr<::SimType> sim(const ::std::string &uri, ::xml_schema::Flags f = 0,
                                 const ::xml_schema::Properties &p = ::xml_schema::Properties());

::std::unique_ptr<::SimType> sim(const ::std::string &uri, ::xml_schema::ErrorHandler &eh, ::xml_schema::Flags f = 0,
                                 const ::xml_schema::Properties &p = ::xml_schema::Properties());

::std::unique_ptr<::SimType> sim(const ::std::string &uri, ::xercesc::DOMErrorHandler &eh, ::xml_schema::Flags f = 0,
                                 const ::xml_schema::Properties &p = ::xml_schema::Properties());

// Parse std::istream.
//

::std::unique_ptr<::SimType> sim(::std::istream &is, ::xml_schema::Flags f = 0,
                                 const ::xml_schema::Properties &p = ::xml_schema::Properties());

::std::unique_ptr<::SimType> sim(::std::istream &is, ::xml_schema::ErrorHandler &eh, ::xml_schema::Flags f = 0,
                                 const ::xml_schema::Properties &p = ::xml_schema::Properties());

::std::unique_ptr<::SimType> sim(::std::istream &is, ::xercesc::DOMErrorHandler &eh, ::xml_schema::Flags f = 0,
                                 const ::xml_schema::Properties &p = ::xml_schema::Properties());

::std::unique_ptr<::SimType> sim(::std::istream &is, const ::std::string &id, ::xml_schema::Flags f = 0,
                                 const ::xml_schema::Properties &p = ::xml_schema::Properties());

::std::unique_ptr<::SimType> sim(::std::istream &is, const ::std::string &id, ::xml_schema::ErrorHandler &eh,
                                 ::xml_schema::Flags f = 0,
                                 const ::xml_schema::Properties &p = ::xml_schema::Properties());

::std::unique_ptr<::SimType> sim(::std::istream &is, const ::std::string &id, ::xercesc::DOMErrorHandler &eh,
                                 ::xml_schema::Flags f = 0,
                                 const ::xml_schema::Properties &p = ::xml_schema::Properties());

// Parse xercesc::InputSource.
//

::std::unique_ptr<::SimType> sim(::xercesc::InputSource &is, ::xml_schema::Flags f = 0,
                                 const ::xml_schema::Properties &p = ::xml_schema::Properties());

::std::unique_ptr<::SimType> sim(::xercesc::InputSource &is, ::xml_schema::ErrorHandler &eh, ::xml_schema::Flags f = 0,
                                 const ::xml_schema::Properties &p = ::xml_schema::Properties());

::std::unique_ptr<::SimType> sim(::xercesc::InputSource &is, ::xercesc::DOMErrorHandler &eh, ::xml_schema::Flags f = 0,
                                 const ::xml_schema::Properties &p = ::xml_schema::Properties());

// Parse xercesc::DOMDocument.
//

::std::unique_ptr<::SimType> sim(const ::xercesc::DOMDocument &d, ::xml_schema::Flags f = 0,
                                 const ::xml_schema::Properties &p = ::xml_schema::Properties());

::std::unique_ptr<::SimType> sim(::xml_schema::dom::unique_ptr<::xercesc::DOMDocument> d, ::xml_schema::Flags f = 0,
                                 const ::xml_schema::Properties &p = ::xml_schema::Properties());

#include <iosfwd>

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/framework/XMLFormatter.hpp>

#include <xsd/cxx/xml/dom/auto-ptr.hxx>

// Serialize to std::ostream.
//

void sim(::std::ostream &os, const ::SimType &x,
         const ::xml_schema::NamespaceInfomap &m = ::xml_schema::NamespaceInfomap(), const ::std::string &e = "UTF-8",
         ::xml_schema::Flags f = 0);

void sim(::std::ostream &os, const ::SimType &x, ::xml_schema::ErrorHandler &eh,
         const ::xml_schema::NamespaceInfomap &m = ::xml_schema::NamespaceInfomap(), const ::std::string &e = "UTF-8",
         ::xml_schema::Flags f = 0);

void sim(::std::ostream &os, const ::SimType &x, ::xercesc::DOMErrorHandler &eh,
         const ::xml_schema::NamespaceInfomap &m = ::xml_schema::NamespaceInfomap(), const ::std::string &e = "UTF-8",
         ::xml_schema::Flags f = 0);

// Serialize to xercesc::XMLFormatTarget.
//

void sim(::xercesc::XMLFormatTarget &ft, const ::SimType &x,
         const ::xml_schema::NamespaceInfomap &m = ::xml_schema::NamespaceInfomap(), const ::std::string &e = "UTF-8",
         ::xml_schema::Flags f = 0);

void sim(::xercesc::XMLFormatTarget &ft, const ::SimType &x, ::xml_schema::ErrorHandler &eh,
         const ::xml_schema::NamespaceInfomap &m = ::xml_schema::NamespaceInfomap(), const ::std::string &e = "UTF-8",
         ::xml_schema::Flags f = 0);

void sim(::xercesc::XMLFormatTarget &ft, const ::SimType &x, ::xercesc::DOMErrorHandler &eh,
         const ::xml_schema::NamespaceInfomap &m = ::xml_schema::NamespaceInfomap(), const ::std::string &e = "UTF-8",
         ::xml_schema::Flags f = 0);

// Serialize to an existing xercesc::DOMDocument.
//

void sim(::xercesc::DOMDocument &d, const ::SimType &x, ::xml_schema::Flags f = 0);

// Serialize to a new xercesc::DOMDocument.
//

::xml_schema::dom::unique_ptr<::xercesc::DOMDocument>
sim(const ::SimType &x, const ::xml_schema::NamespaceInfomap &m = ::xml_schema::NamespaceInfomap(),
    ::xml_schema::Flags f = 0);

void operator<<(::xercesc::DOMElement &, const ArgsType &);

void operator<<(::xercesc::DOMElement &, const PositionType &);

void operator<<(::xercesc::DOMElement &, const VelocityType &);

void operator<<(::xercesc::DOMElement &, const SizeType &);

void operator<<(::xercesc::DOMElement &, const CuboidType &);

void operator<<(::xercesc::DOMElement &, const ObjectsType &);

void operator<<(::xercesc::DOMElement &, const SimType &);

#include <xsd/cxx/post.hxx>

// Begin epilogue.
//
//
// End epilogue.

#endif // SIMULATION_XSD_H
