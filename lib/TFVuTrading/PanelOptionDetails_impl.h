/************************************************************************
 * Copyright(c) 2014, One Unified. All rights reserved.                 *
 * email: info@oneunified.net                                           *
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

#pragma once

#include <boost/fusion/container/vector/vector20.hpp>
#include <boost/fusion/include/vector20.hpp>

#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/algorithm/iteration/fold.hpp>
#include <boost/fusion/include/fold.hpp>
#include <boost/fusion/sequence/intrinsic/at_c.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/algorithm/transformation/filter.hpp>
#include <boost/fusion/include/filter.hpp>

#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/grid.h>

#include <TFVuTrading/ModelCell.h>
#include <TFVuTrading/ModelCell_ops.h>
#include <TFVuTrading/ModelCell_macros.h>

#include "PanelOptionDetails.h"

namespace ou { // One Unified
namespace tf { // TradeFrame

struct WinOptionDetails_impl {
//public:
  WinOptionDetails_impl( WinOptionDetails& );
  ~WinOptionDetails_impl( void );
//protected:

//private:

  WinOptionDetails& m_details;

// for column 2, use wxALIGN_LEFT, wxALIGN_CENTRE or wxALIGN_RIGHT
#define GRID_ARRAY_PARAM_COUNT 5
#define GRID_ARRAY_COL_COUNT 13
#define GRID_ARRAY \
  (GRID_ARRAY_COL_COUNT,  \
    ( /* Col 0,         1,            2,         3,      4,             */ \
      (COL_CallBid  , "Bid",   wxALIGN_LEFT,  100, ModelCellDouble ), \
      (COL_CallLast , "Last",  wxALIGN_LEFT,  50, ModelCellDouble ), \
      (COL_CallAsk  , "Ask",   wxALIGN_LEFT,   50, ModelCellDouble ), \
      (COL_CallIV   , "IV",    wxALIGN_LEFT,  50, ModelCellDouble ), \
      (COL_CallDelta, "Delta", wxALIGN_LEFT,   50, ModelCellDouble ), \
      (COL_CallGamma, "Gamma", wxALIGN_LEFT,  60, ModelCellDouble ), \
      (COL_Strike   , "Strk",  wxALIGN_LEFT,  60, ModelCellDouble ), \
      (COL_PutBid   , "Bid",   wxALIGN_LEFT,  100, ModelCellDouble ), \
      (COL_PutLast  , "Last",  wxALIGN_LEFT,  50, ModelCellDouble ), \
      (COL_PutAsk   , "Ask",   wxALIGN_LEFT,   50, ModelCellDouble ), \
      (COL_PutIV    , "IV",    wxALIGN_LEFT,  50, ModelCellDouble ), \
      (COL_PutDelta , "Delta", wxALIGN_LEFT,   50, ModelCellDouble ), \
      (COL_PutGamma , "Gamma", wxALIGN_LEFT,  60, ModelCellDouble ), \
      ) \
    ) \
  /**/

  enum {
    BOOST_PP_REPEAT(GRID_ARRAY_COL_COUNT,GRID_EXTRACT_ENUM_LIST,0)
  };

  typedef boost::fusion::VECTOR_DEF<
    BOOST_PP_REPEAT(GRID_ARRAY_COL_COUNT,COMPOSE_MODEL_CELL,4)
  > vModelCells_t;
  
  class OptionValueRow {
  public:
    OptionValueRow( wxGrid* pGrid, int nRow ): m_pGrid( pGrid ), m_nRow( nRow ) { Init(); }
    OptionValueRow( const OptionValueRow& rhs ): m_pGrid( rhs.m_pGrid ), m_nRow( rhs.m_nRow ) { Init(); }
    ~OptionValueRow( void ) {}
    
    void UpdateGui( void ) {
      boost::fusion::for_each( m_vModelCells, ModelCell_ops::UpdateGui( m_pGrid, m_nRow ) );
    }
    void UpdateCallGreeks( ou::tf::Greek& greek ) {
      boost::fusion::at_c<COL_CallIV>( m_vModelCells ).SetValue( greek.ImpliedVolatility() );
      boost::fusion::at_c<COL_CallDelta>( m_vModelCells ).SetValue( greek.Delta() );
      boost::fusion::at_c<COL_CallGamma>( m_vModelCells ).SetValue( greek.Gamma() );
    }
    void UpdateCallQuote( ou::tf::Quote& quote ) {
      boost::fusion::at_c<COL_CallBid>( m_vModelCells ).SetValue( quote.Bid() );
      boost::fusion::at_c<COL_CallAsk>( m_vModelCells ).SetValue( quote.Ask() );
    }
    void UpdateCallTrade( ou::tf::Trade& trade ) {
      boost::fusion::at_c<COL_CallLast>( m_vModelCells ).SetValue( trade.Price() );
    }
    void UpdatePutGreeks( ou::tf::Greek& greek ) {
      boost::fusion::at_c<COL_PutIV>( m_vModelCells ).SetValue( greek.ImpliedVolatility() );
      boost::fusion::at_c<COL_PutDelta>( m_vModelCells ).SetValue( greek.Delta() );
      boost::fusion::at_c<COL_PutGamma>( m_vModelCells ).SetValue( greek.Gamma() );
}
    void UpdatePutQuote( ou::tf::Quote& quote ) {
      boost::fusion::at_c<COL_PutBid>( m_vModelCells ).SetValue( quote.Bid() );
      boost::fusion::at_c<COL_PutAsk>( m_vModelCells ).SetValue( quote.Ask() );
    }
    void UpdatePutTrade( ou::tf::Trade& trade ) {
      boost::fusion::at_c<COL_PutLast>( m_vModelCells ).SetValue( trade.Price() );
    }
  protected:
  private:
    wxGrid* m_pGrid;
    int m_nRow;
    vModelCells_t m_vModelCells;
    
    void Init( void ) {
      boost::fusion::fold( m_vModelCells, 0, ModelCell_ops::SetCol() );
      BOOST_PP_REPEAT(GRID_ARRAY_COL_COUNT,COL_ALIGNMENT,m_nRow)
    }
  };
  
  typedef std::map<double,OptionValueRow> mapOptionValueRow_t;
  typedef mapOptionValueRow_t::iterator mapOptionValueRow_iter;
  mapOptionValueRow_t m_mapOptionValueRow;

  mapOptionValueRow_iter FindOptionValueRow( double );
  
  wxGrid* m_pGrid;  // for use in macro GRID_EMIT_SetColSettings
  
  void UpdateCallGreeks( double strike, ou::tf::Greek& );
  void UpdateCallQuote( double strike, ou::tf::Quote& );
  void UpdateCallTrade( double strike, ou::tf::Trade& );  
  void UpdatePutGreeks( double strike, ou::tf::Greek& );
  void UpdatePutQuote( double strike, ou::tf::Quote& );
  void UpdatePutTrade( double strike, ou::tf::Trade& );  

  void CreateControls();
  void OnClose( wxCloseEvent& event );

  template<typename Archive>
  void save( Archive& ar, const unsigned int version ) const {
    int cnt = m_pGrid->GetCols();
    ar & cnt;
    for ( int ix = 0; ix < cnt; ix++ ) {
      ar & m_pGrid->GetColumnWidth( ix );
    }
  }

  template<typename Archive>
  void load( Archive& ar, const unsigned int version ) {
    int cnt;
    ar & cnt;
    assert( cnt == m_pGrid->GetCols() ); 
    int width;
    for ( int ix = 0; ix < cnt; ix++ ) {
      ar & width;
      m_pGrid->SetColumnWidth( ix, width );
    }
  }

  BOOST_SERIALIZATION_SPLIT_MEMBER()
};

template<class Archive>
void WinOptionDetails::serialize(Archive & ar, const unsigned int file_version){
    ar & *m_pimpl;
}  

} // namespace tf
} // namespace ou