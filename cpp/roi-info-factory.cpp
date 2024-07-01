#include "../headers/roi-info-factory.h"
#include <stdio.h>
#include <iostream>

// C-Tor
RoiInfoFactory::RoiInfoFactory(unsigned long cols, unsigned long rows, unsigned int cn):
cols(cols), rows(rows), channels(cn)
{}


// D-Tor
RoiInfoFactory::~RoiInfoFactory(){}

RoiInfo RoiInfoFactory::create(const unsigned long start_row, const unsigned long end_row, const unsigned long start_col, const unsigned long end_col){
    
    RoiInfo inf;
    inf.cn = this->channels;
    inf.cols = this->cols;
    inf.rows = this->rows;

    // sanity checks
    inf.bad = sanityChecks(end_row, end_col);
     
    if ( inf.bad ){
        return inf;
    } 

    // build info
    inf.start_col = start_col;
    inf.end_col   = end_col;

    inf.start_row = start_row;
    inf.end_row   = end_row;

    return inf;
}

bool RoiInfoFactory::sanityChecks(const unsigned long end_row, const unsigned long end_col){
    bool result = (end_row > this->rows)||(end_col >= this->cols);

    if(result)
    {
        std::cout <<"ERROR --BAD PARAMS-- :\n"<< "rows : " << std::to_string(this->rows) << " | cols : " << std::to_string(this->cols) << "\n"
        << " | end_row : " << std::to_string(end_row) << " | end_cols : " << std::to_string(end_col) << std::endl;
    }
    return result;
}