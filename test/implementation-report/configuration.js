var state

function init_state()
{
     state = { "header": {
        "prefix": [
            { "dc": "<http://purl.org/dc/terms/>" },
            { "earl": "<http://www.w3.org/ns/earl#>" },
            { "doap": "<http://usefulinc.com/ns/doap#>" },
            { "foaf": "<http://xmlns.com/foaf/0.1/>" },
            { "xsd": "<http://www.w3.org/2001/XMLSchema#>" }
        ]}
    }
}
function doap()
{
    init_state()
    let uri = document.getElementById("project_uri").value
    let doap = document.getElementById("doap")
    let primary = document.getElementById("foaf-primary")
    let prefix = document.getElementById("prefix")

    if (uri !== "") {
        doap.innerHTML = `${uri} a doap:Project;`
        let today = new Date();

        primary.innerHTML = ` foaf:primaryTopic ${uri}<br
            />&nbsp;&nbsp;dc:issued "${today.toISOString()}"^^csd:dateTime;`
        state.header.subject = []
        state.header.subject[0] =
            { "id" : "<>",
              "type": "http://xmlns.com/foaf/0.1/primaryTopic",
              "value": uri }
        state.header.subject[0].properties = []

    } else {
        doap.innerHTML = "<p style=\"text-style: italic;\">Project URI Required</p>"
        return -1;
    }

    let name = document.getElementById("project_name").value
    if (name !== "") {
           doap.innerHTML += `<br />&nbsp;&nbsp;doap:name "${name}";`
           state.header.subject[1] = { "id": uri,
                "type": "a",
                "value": "http://usefulinc.com/ns/doap#project",
                "properties": [
                    { "type": "http://usefulinc.com/ns/doap#name", "value": name }
                ]
           }
    }
    
    let desc = document.getElementById("desc").value
    if (desc !== "") {
           doap.innerHTML += `<br />&nbsp;&nbsp;doap:description"${desc}"@en;`
           state.header.subject[1].properties.push({
               "type": "http://usefulinc.com/ns/doap#description",
               "value": desc})
    }

    let homepage = document.getElementById("homepage").value
    if (homepage !== "") {
           doap.innerHTML += `<br />&nbsp;&nbsp;doap:homepage "${homepage}"`
           state.header.subject[1].properties.push({
               "type": "http://usefulinc.com/ns/doap#homepage",
               "value": homepage})
    }
    let developers = document.getElementById("developers")
    let developer_list = developers.getElementsByTagName("form")
    for (let i = 0; i < developer_list.length; i++){
        let developer_uri = developer_list[i].getElementsByClassName("github-profile")[0].value
        let select = developer_list[i].getElementsByClassName("developer-type")[0]
        let developer_type = select.options[select.selectedIndex].text
        if ( developer_uri !== "") {
            doap.innerHTML += `<br />&nbsp;&nbsp;doap:${developer_type} ${developer_uri}`
            if ( developer_type == "maker"){
                primary.innerHTML +=`<br/>&nbsp;&nbsp;foaf:${developer_type} ${developer_uri}`
                state.header.subject[0].properties.push({
                    "type": `http://xmlns.com/foaf/0.1/maker`,
                    "value": developer_uri })
            } else {
                state.header.subject[1].properties.push({
                    "type": `http://usefulinc.com/ns/doap#${developer_type}`,
                    "value": developer_uri })
            }
        }

    }

    primary.innerHTML += "<br />&nbsp;&nbsp;."
    doap.innerHTML += "<br />&nbsp;&nbsp;."
}

function addDeveloper(){
    console.log(`adding developer`)
    let developers = document.getElementById("developers")
    let template = document.getElementById("developer-template")
    let developer = template.content.cloneNode(true)
    developers.appendChild(developer)
    
    let update_buttons = developers.getElementsByClassName("dev-update-button")
    let update_button = update_buttons[update_buttons.length -1]
    update_button.addEventListener('click', function(e){
        update()
    }, false)
    
    let del_buttons = developers.getElementsByClassName("dev-del-button")
    let del_button = del_buttons[del_buttons.length -1]
    del_button.addEventListener('click', function(e){
        let source = e.target || e.srcElement
        source.parentNode.remove()
    }, false)
}

function addTestsuite(){
    console.log(`adding testsuite`)
    let testsuites = document.getElementById("testsuites")
    let template = document.getElementById("testsuite-template")
    let testsuite = template.content.cloneNode(true)
    testsuites.appendChild(testsuite)
    
    let update_buttons = testsuites.getElementsByClassName("dev-update-button")
    let update_button = update_buttons[update_buttons.length -1]
    update_button.addEventListener('click', function(e){
        update()
    }, false)
    
    let del_buttons = testsuites.getElementsByClassName("dev-del-button")
    let del_button = del_buttons[del_buttons.length -1]
    del_button.addEventListener('click', function(e){
        let source = e.target || e.srcElement
        source.parentNode.remove()
    }, false)
}

function update()
{
    doap()
    foaf()
    testsuites()
}

function foaf()
{
    let foaf = document.getElementById("foaf")
    foaf.innerHTML = ""
    let developers = document.getElementById("developers")
    let developer_list = developers.getElementsByTagName("form")
    if (developer_list.length > 0) {
        for (let i = 0; i < developer_list.length; i++){
            //create element
            let developer = document.createElement("p")
            foaf.appendChild(developer)
            let developer_uri = developer_list[i].getElementsByClassName("github-profile")[0].value
            let developer_name = developer_list[i].getElementsByClassName("developer-name")[0].value
            if (developer_name !== "") {
                developer.innerHTML += `${developer_uri} a foaf:Person; <br
                    />\&nbsp;&nbsp;foaf:name "${developer_name}";<br />&nbsp;&nbsp;.`
                state.header.subject.push({
                    "id": developer_uri,
                    "type": "a", "value": "http://xmlns.com/foaf/0.1/Person",
                    "properties": [ { "type": "http://xmlns.com/foaf/0.1/name",
                        "value": developer_name } ] })
            }
        }
    }
}

function testsuites() {
    state.testsuites = []
    let testsuites = document.getElementById("testsuites")
    let testsuite_list = testsuites.getElementsByTagName("form")
     if (testsuite_list.length > 0) {
        for (let i = 0; i < testsuite_list.length; i++ ){
            let testsuite = testsuite_list[i].getElementsByTagName("input")[0].files[0]
            let manifest = testsuite_list[i].getElementsByTagName("input")[1].value
            state.testsuites.push({ "file": testsuite.name, "manifest": manifest})
        }
    }
}

function download() {
    let a = document.getElementById("download")
    let text = document.getElementById("result").innerText
    let type = "application/x-turtle"
    let filename = document.getElementById("project_name").value
    console.log(`filename: ${filename} text: ${text}`)
    if (filename == "") filename = "myproject"
    filename += ".doap"
    let file = new Blob([text], {type: type})
    a.href = URL.createObjectURL(file)
    a.download = filename
}

function download_json() {
    let a = document.getElementById("download-json")
    let type = "application/json"
    let filename = document.getElementById("project_name").value
    if (filename == "") filename = "myproject"
    filename += ".json"
    let file = new Blob([JSON.stringify(state)], {type: type})
    a.href = URL.createObjectURL(file)
    a.download = filename
}
