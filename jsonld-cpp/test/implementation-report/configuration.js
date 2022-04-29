var state
const debug = false

const developer_titles = [
    "maker",
    "developer",
    "documenter",
    "maintainer",
    "creator"
]

const doapProperties = [
    {
        "id": "project-uri",
        "display": "uri",
        "default": "https://github.com/dcdpr/jsonld-cpp"
    },
    {
        "id": "project-name",
        "display": "name",
        "default": "JSONLD-CPP"
    },
    {
        "id": "project-description",
        "display": "description",
        "default": "A JSON-LD 1.1 Processor & API for C++"
    },
    {
        "id": "project-homepage",
        "display": "homepage",
        "default": "https://github.com/dcdpr/jsonld-cpp"
    },
    {
        "id": "project-organization",
        "display": "organization",
        "default": "https://github.com/dcdpr"
    },
    {
        "id": "project-programming-language",
        "display": "language",
        "default": "C++"
    }
]

const doapVersionProperties = [
    {
        "id": "version-name",
        "display": "name",
        "default": "jsonld-cpp"
    },
    {
        "id": "version-created",
        "display": "created",
        "default": "@generatedate@"
    },
    {
        "id": "version-revision",
        "display": "revision",
        "default": "0.9"
    },
]

/**
 *  Add project property fields to the form
 */
function addProjectProperties() {
    let project = document.getElementById( "project" )
    let template = document.getElementById( "project-property-template" )
    addProperty( doapProperties, project, template )
    let version = document.getElementById( "version" )
    addProperty( doapVersionProperties, version, template )
}

function addProperty( property_list, parent_node, template ){
    property_list.forEach( ( project_property ) => {
        /**
         * Create a deep clone of the template and get the HTML elements to
         * populate
         */
        let property = template.content.cloneNode(true)
        /**
         * Add the property to the form
         */
        parent_node.appendChild( property )
        /**
         * Get the added property label
         */
        let property_label = document.querySelectorAll( ".property-label" )
        let property_input = document.querySelectorAll( ".property-input" )
        let label = property_label[ property_label.length - 1 ]
        let input = property_input[ property_input.length - 1 ]
        /**
         * Create the for attribute for the property label
         */
        let attribute_for = document.createAttribute( 'for' )
        attribute_for.value = project_property.id
        label.setAttributeNode( attribute_for )
        /**
         * Create the text the property label
         */
        let text_label = document.createTextNode( project_property.display )
        label.appendChild( text_label )
        /**
         * Create the id attribute for the property input
         */
        let attribute_id = document.createAttribute( 'id' )
        attribute_id.value = project_property.id
        input.setAttributeNode( attribute_id )
        /**
         *  Create the name attribute for the property input
         */
        let attribute_name = document.createAttribute( 'name' )
        attribute_name.value = project_property.id
        input.setAttributeNode( attribute_name )
        /**
         * Create the value attribute for the property input
         */
        let attribute_value = document.createAttribute( 'value' )
        attribute_value.value = project_property.default
        input.setAttributeNode( attribute_value )
    })
}

/**
 * Dynamically add a developer row to the form
 */
function addDeveloper(){
    let developers = document.getElementById("developers")
    let template = document.getElementById("developer-template")
    let developer = template.content.cloneNode(true)
    developers.appendChild(developer)
    
    let del_buttons = developers.getElementsByClassName("dev-del-button")
    let del_button = del_buttons[del_buttons.length -1]
    del_button.addEventListener('click', function(e){
        let source = e.target || e.srcElement
        source.parentNode.remove()
    }, false)
}

/**
 * Dynamically add a testsuite row to the form
 */
function addTestsuite(){
    let testsuites = document.getElementById("testsuites")
    let template = document.getElementById("testsuite-template")
    let testsuite = template.content.cloneNode(true)
    testsuites.appendChild(testsuite)
    
    let del_buttons = testsuites.getElementsByClassName("dev-del-button")
    let del_button = del_buttons[del_buttons.length -1]
    del_button.addEventListener('click', function(e){
        let source = e.target || e.srcElement
        source.parentNode.remove()
    }, false)
}

/**
 *  Load an existing cnfiguration file
 */
function load_json( fileInput ) {

    if ( fileInput.files[0] == undefined ){
        return
    }

    let reader = new FileReader()
    /**
     * This event is triggered when the file is loaded
     */
    reader.onload = function( ev ) {
        let content = ev.target.result
        state = JSON.parse( content )
        update_form()
    }
    reader.onerror = function( err ) {
        if ( debug ) console.error( "Failed to read file", err )
    }
    reader.readAsText( fileInput.files[0] )
}

/**
 * Updates the form with the state content
 */
function update_form(){

    /**
     *  Remove all developers and testsuites
     */
    let templateForms = document.getElementsByClassName( 'template-form' )
    /**
     *  As we remove elements the length dcreases so we need to initialise the
     *  iterator at the length and then decrement the iterator to -1 to
     *  terminte the loop
     */
    for ( let i = templateForms.length - 1 ; i >= 0; --i )
    {
        templateForms[i].remove()
    }

    /**
     *  Update the properties
     */
    let properties = state.header.subject[1].properties
    for ( let i = 0; i < properties.length; i++ ){

        let property = properties[ i ]

        if ( debug ) console.log( property.id )
        let element_name = property.type.replace( /^.*#/,'project-' )
        let element_name_no_prefix = property.type.replace( /^.*#/,'' )
        let developers = state.header.subject
        if ( debug ) console.log( `element name: ${element_name} (without prefix: ${element_name_no_prefix})` )
        if ( debug ) console.log( developer_titles )
        /**
         * If the property is a developer property then we need to process this
         * with the developers section, adding a developer
         */
        if ( debug ) console.log( `Adding property: ${ element_name }` )
        if ( developer_titles.includes( element_name_no_prefix ) ){
            /**
             *  Add a developer form element
             */
            addDeveloper()
            /**
             *  Find the developer in the developers sections of the json
             */
            let developer
            for ( let ii = 2; ii < developers.length; ii++ )
            {
                developer = developers[ii]
                if ( debug ) console.log( `developer: ${developer.id} property: ${property.value}` )
                if ( developer.id != property.value ) continue

                /**
                 * Get the last element with the github-profile class
                 * Get the last element with the developer-name class
                 * and the last element with the developer-type class
                 */
                let github_profile = get_last_element( 'github-profile' ) 
                let developer_name = get_last_element( 'developer-name' ) 
                let developer_type = get_last_element( 'developer-type' ) 
                developer_name.value = developer.properties[0].value
                github_profile.value = developer.id
                /**
                 * Select the developer type
                 */
                let options = Array.from( developer_type.options )
                let option_to_select = options.find( item => item.text === element_name_no_prefix )
                option_to_select.selected = true

                if ( debug ) console.log( `set name: ${developer.properties[0].value} url: ${developer.id} type: ${element_name_no_prefix}` )
            }
        }
        /**
         * If the property is not a developer then we locate the correct
         * form element for it and add the value
        */
        else
        {
            let element = document.getElementById( element_name )
            if ( debug ) console.log( `element.value ${element.value} element_name ${element_name}`)
            element.value = property.value
        }
    }

    /**
     *  Update the testsuites
     */

    let testsuites = state.testsuites
    for ( let i = 0; i < testsuites.length; i++ ){
        let testsuite = testsuites[i]
        addTestsuite()
        let manifest_name = get_last_element( 'manifest-name' )
        let manifest_file = get_last_element( 'manifest-file' )
        manifest_name.value = testsuite.manifest
        manifest_file.style.visibility = "hidden"
        let para = document.createElement( 'p' )
        para.classList.add( 'manifest-file-import' )
        let textnode = document.createTextNode( testsuite.file )
        para.appendChild( textnode )
        manifest_file.parentNode.insertBefore( para, manifest_file.nextSibling )
    }

    /**
     *  Update the preview area
     */
    let preview = document.getElementById("preview")
    preview.innerHTML = JSON.stringify( state, null, 4 )
        .replaceAll('<','&lt;')
        .replaceAll('>','&gt;')
}

function get_last_element( class_name ) {
    let results = document.getElementsByClassName( class_name )
    return results[results.length - 1]
}

/**
 * Updates the state variable with the form content
 */
function update_state()
{
    /**
     * We first initialise the state variable with the header section and the
     * prefix that are going to be used in the implementation report.
     * These are JSON representations of the RdfNamespace class instances
     */
     state = { "header": {
        "prefix": [
            { "dc": "<http://purl.org/dc/terms/>" },
            { "earl": "<http://www.w3.org/ns/earl#>" },
            { "doap": "<http://usefulinc.com/ns/doap#>" },
            { "foaf": "<http://xmlns.com/foaf/0.1/>" },
            { "xsd": "<http://www.w3.org/2001/XMLSchema#>" }
        ]}
    }

    state.header.subject = []
    const primary_topic = 0
    /**
     * The next piece of data we need to encode is the Primary Topic which is a
     * JSON representation of a RdfData instance.  
     */
    let uri = document.getElementById("project-uri").value
    if (uri !== "") {
        state.header.subject[ primary_topic ] = {
              "id" : "<>",
              "type": "foaf:primaryTopic",
              "value": uri,
              "properties": [
                  {
                      "type": "dc:issued",
                      "value": "@generatedtime@"
                  }
              ]
        }
    }

    /**
     * We need to create a description of a project using the several elements
     * from the form.  These are added to the properties of the URI that
     * is defined as the Primary Topic above
     */
    const doap_index = state.header.subject.length
    state.header.subject[ doap_index ] = {
        "id": uri,
        "type": "a",
        "value": "doap:project",
        "properties": []
    }

    doapProperties.forEach( ( property ) => {
        if ( debug ) console.log( property )
        addDoapProperty( state.header.subject[ doap_index ], property.display, property.id  )
    })

    /**
     * We need to have a nested release section
     */
    let doap_version_index = state.header.subject.length
    state.header.subject[ doap_version_index ]  = {
        "id": "doap:release",
        "type": "a",
        "value": "doap:Version",
        "properties": []
    }

    doapVersionProperties.forEach( ( property ) => {
        addDoapProperty( state.header.subject[ doap_version_index], property.display, property.id )
    })

    function addDoapProperty( property_list, name, id ){
        try {
            if ( debug ) console.log( `addDoapProperty ${name} : ${id}` )
            let property = document.getElementById( id ).value
            if (property !== "") {
                property_list.properties.push({
                    "type": `doap:${name}`,
                    "value": property
                })
            }
        } catch( e ) {
            console.log( e )
        }
    }

    /**
     * The next data to add to the state is a set of developers.
     */
    let developers = document.getElementById("developers")
    let developer_list = developers.getElementsByTagName("form")
    let dev_set = []
    for (let i = 0; i < developer_list.length; i++){
        let developer_uri = developer_list[i].getElementsByClassName("github-profile")[0].value
        let developer_name = developer_list[i].getElementsByClassName("developer-name")[0].value
        let select = developer_list[i].getElementsByClassName("developer-type")[0]
        let developer_type = select.options[select.selectedIndex].text
        if ( debug ) console.log( `Updating state with developer: ${developer_uri}/${developer_type}` )
        if ( developer_uri !== "") {
            /**
             * If the developer type is maker then this entry belongs in the
             * Primary Topic subject
             */
            if ( developer_type == "maker"){
                state.header.subject[ primary_topic ].properties.push({
                    "type": `http://xmlns.com/foaf/0.1/maker`,
                    "value": developer_uri
                })
            /**
             * Otherwise we add the developer to the doap section
             */
            } else {
                state.header.subject[ doap_index ].properties.push({
                    "type": `doap:${developer_type}`,
                    "value": developer_uri
                })
            }
        }

        /**
         * If the developer has already been added once then we don't need to
         * perform any more actions so exit the loop
         */
        if ( dev_set.includes( developer_uri ) ){
            if ( debug ) console.log( `Already defined developer` )
            continue 
        }

        /**
         *  Add the developer_uri to the set of known developers
         */
        dev_set.push( developer_uri )

        /**
         *  Create a friend of a friend (foaf) entry if the developer
         */
        if (developer_name !== "") {
            state.header.subject.push({
                "id": developer_uri,
                "type": "a", "value": "foaf:Person",
                "properties": [{
                    "type": "foaf:name",
                    "value": developer_name
                }]
            })
        }
    }

    /**
     * Now we update the state with the testsuites that have been defined
     */
    state.testsuites = []
    let testsuites = document.getElementById("testsuites")
    let testsuite_list = testsuites.getElementsByTagName("form")
    for (let i = 0; i < testsuite_list.length; i++ ){
       let testsuite
       try {
            testsuite = testsuite_list[i].getElementsByTagName("input")[0].files[0].name
       } catch ( e ) {
            testsuite = get_last_element( 'manifest-file-import' ).textContent
           if ( debug ) console.log( testsuite )
       }
       let manifest = testsuite_list[i].getElementsByTagName("input")[1].value
       state.testsuites.push({ "file": testsuite, "manifest": manifest})
    }

    /**
     *  Update the preview area
     */
    let preview = document.getElementById("preview")
    preview.innerHTML = JSON.stringify( state, null, 4 )
        .replaceAll('<','&lt;')
        .replaceAll('>','&gt;')

    /**
     * Update downloadable blob
     */
    let a = document.getElementById("download-json")
    let type = "application/json"
    let filename = document.getElementById("project-name").value
    if (filename == "") filename = "myproject"
    filename += ".json"
    let file = new Blob([JSON.stringify(state)], {type: type})
    a.href = URL.createObjectURL(file)
    a.download = filename
}

