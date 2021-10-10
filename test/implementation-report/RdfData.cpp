#include "RdfData.h"

void RdfData::addChild( RdfData* d )
{
    objects.push_back( d );
}

void RdfData::addChild( RdfObject& o )
{
    addChild( new RdfData( o ) );
}

void RdfData::addChild( RdfObject& o, RdfData* d )
{
    objects.push_back( new RdfData( o, d ) );
}

