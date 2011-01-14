/************************************************************************
 * Copyright(c) 2010, One Unified. All rights reserved.                 *
 *                                                                      *
 * This file is provided as is WITHOUT ANY WARRANTY                     *
 *  without even the implied warranty of                                *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                *
 *                                                                      *
 * This software may not be used nor distributed without proper license *
 * agreement.                                                           *
 *                                                                      *
 * See the file LICENSE.txt for redistribution information.             *
 ************************************************************************/

// LibSqlite/Actions.h

#pragma once

#include <stdexcept>

#include <boost/cstdint.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>

#include <LibSQL\Actions.h>

#include "StatementState.h"

namespace ou {
namespace db {
namespace sqlite {

class Action_Assemble_TableDef: public ou::db::Action_Assemble_TableDef {
public:

  const char* FieldType( char key );
  const char* FieldType( bool key );
  const char* FieldType( boost::int64_t key );
  const char* FieldType( boost::int32_t key );
  const char* FieldType( boost::int16_t key );
  const char* FieldType( boost::int8_t key );
  const char* FieldType( std::string& key );
  const char* FieldType( double key );
  const char* FieldType( boost::posix_time::ptime& key ); // don't use julian as ptime has no representation earlier than 1400 AD

  Action_Assemble_TableDef( const std::string& sTableName ): ou::db::Action_Assemble_TableDef( sTableName ) {};
  ~Action_Assemble_TableDef( void ) {};

  template<typename T>
  void Field( const std::string& sFieldName, T& var ) {
    addField( sFieldName, FieldType( var ) );
  }

  template<typename T>
  void Field( const std::string& sFieldName, T& var, const std::string& sFieldType ) {
    addField( sFieldName, sFieldType.c_str() );
  }

protected:
private:
};

class Action_Bind_Values {
public:

  Action_Bind_Values( structStatementState& state ): m_state( state ), m_index( 0 ) {};
  ~Action_Bind_Values( void ) {};

  int Bind( char var );
  int Bind( bool var );
  int Bind( boost::int64_t var );
  int Bind( boost::int32_t var );
  int Bind( boost::int16_t var );
  int Bind( boost::int8_t var );
  int Bind( std::string& var );
  int Bind( double var );
  int Bind( boost::posix_time::ptime& var );

  template<typename T>
  void Field( const std::string& sFieldName, T& var, const std::string& sFieldType = "" ) {
    ++m_index;
    int rtn = Bind( var );
    if ( SQLITE_OK != rtn ) {
      std::string sErr( "Action_Bind_Values::Field::Bind: " );
      sErr += boost::lexical_cast<std::string>( rtn );
      sErr += ")";
      throw std::runtime_error( sErr );
    }
  }

protected:
private:
  structStatementState m_state;
  int m_index;  // index into sql statement, starts at 1
};

// 
// dealing with key resolution: find db field type from c++ plain old data type
//

// following functions are not used.  Need to fix the enumeration problem first.
template<typename T>
const char* FieldType2( void ) { // is called with enumerations, so need to figure out appropriate type conversion
  std::string s;
  s += "FieldType2 bad cast: ";
  s += typeid( T ).name();
  throw std::runtime_error( s ); 
};
template<> const char* FieldType2<char>( void );
template<> const char* FieldType2<bool>( void );
template<> const char* FieldType2<boost::int64_t>( void );
template<> const char* FieldType2<boost::int32_t>( void );
template<> const char* FieldType2<boost::int16_t>( void );
template<> const char* FieldType2<boost::int8_t>( void );
template<> const char* FieldType2<std::string>( void );
template<> const char* FieldType2<double>( void );
// don't use julian as ptime has no representation earlier than 1400 AD
template<> const char* FieldType2<boost::posix_time::ptime>( void );



} // sqlite
} // db
} // ou
