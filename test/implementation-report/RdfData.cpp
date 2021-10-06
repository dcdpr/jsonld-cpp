#include "RdfData.h"

void RdfData::addChild( RdfData* d )
{
    objects.insert( d );
}

void RdfData::addChild( RdfObject& o )
{
    addChild( new RdfData( o ) );
}

void RdfData::addChild( RdfObject& o, RdfData* d )
{
    objects.insert( new RdfData( o, d ) );
}

