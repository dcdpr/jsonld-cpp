#include "RdfData.h"
#include <iostream>

RdfObject RdfData::getValue()
{
    RdfObject result;
    for ( auto o : objects )
    {
        if ( o->objects.empty() )
            return o->subject;
    }
    return result;
}
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

