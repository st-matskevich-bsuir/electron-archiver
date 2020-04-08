const $ = require('jquery');

var selectedFile = null;
var compressMode = true;

$.fn.enterKey = function (callback) {
    return this.each(function () {
        $(this).keypress(function (event) {
            var keycode = (event.keyCode ? event.keyCode : event.which);
            if (keycode == '13') {
                callback.call(this, event);
            }
        })
    })
};

function onFileSelected(files) {
    if (files.length > 0) {
        selectedFile = files[0];
        $('#file_container .title').text(selectedFile.name);
        $('#process_button').toggleClass('disabled', false);
        $('#process_button').text('Process');
    } else {
        selectedFile = null;
        $('#file_container .title').text('Drop file here or click for selection');
        $('#process_button').toggleClass('disabled', true);
        $('#process_button').text('No file selected');
    }
}

function ProcessFile() {
    $('#controls_wrapper').hide();
    $('#preloader').show();

    if (selectedFile != null) {
        console.log(selectedFile.path);
        if(Compressor == null || Compressor == undefined)
            OnCompressorDone(-4);

        if (compressMode) {
            result = Compressor.compress(selectedFile.path, OnCompressorDone);
        } else {
            result = Compressor.decompress(selectedFile.path, OnCompressorDone);
        }
    }
}

function OnCompressorDone(result){
    console.log(result);
    $('#result_wrapper').show();
    $('#preloader').hide();
    $('#result_icon').toggleClass('complete', result == 0);
    $('#result_icon').toggleClass('error', result != 0);
    switch(result)
    {
        case 0:
            $('#result_wrapper .title').text("File successfuly processed");
            break;
        case -1:
            $('#result_wrapper .title').text("Process failed. Can't open selected file");
            break;
        case -2:
            $('#result_wrapper .title').text("Process failed. Can't write processed file");
            break;
        default:
            $('#result_wrapper .title').text("Process failed with unknown error. Code: " + result);
            break;
    }
}

$('#exit_button').click(() => {
    window.close();
});

$('#mode_toggle').click(() => {
    $('#mode_toggle').toggleClass('on');
    compressMode = !compressMode;
});

$('#mode_toggle').enterKey(() => {
    $('#mode_toggle').toggleClass('on');
    compressMode = !compressMode;
});

$('#file_container').click(() => {
    $('#selected_file').click();
});

$('#file_container').enterKey(() => {
    $('#selected_file').click();
});

$('#file_container').on('dragover', (event) => {
    event.preventDefault();
    event.stopPropagation();
    $('#file_container').toggleClass('drag-over', true);
});

$('#file_container').on('dragleave', (event) => {
    event.preventDefault();
    event.stopPropagation();
    $('#file_container').toggleClass('drag-over', false);
});

$('#file_container').on('drop', (event) => {
    event.preventDefault();
    event.stopPropagation();
    $('#file_container').toggleClass('drag-over', false);
    onFileSelected(event.originalEvent.dataTransfer.files);
});

$('#selected_file').change(() => {
    onFileSelected($('#selected_file')[0].files);
});


$('#process_button').click(() => {
    ProcessFile();
});

$('#process_button').enterKey(() => {
    ProcessFile();
});

$('#result_button').click(() => {
    $('#controls_wrapper').show();
    $('#result_wrapper').hide();
});

$('#result_button').enterKey(() => {
    $('#controls_wrapper').show();
    $('#result_wrapper').hide();
});