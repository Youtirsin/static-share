const { createApp } = Vue

window.onload = function () {
  createApp({
    methods: {
      constructPath() {
        this.current_path = "/"
        for (let i = 0; i < this.path_stack.length; ++i)
          this.current_path += this.path_stack[i] + "/"
      },
      goToPath(dir) {
        this.path_stack.push(dir)
        this.history_stack = []
        this.constructPath()
        loadFileInfo(this.current_path, this.handleFileList)
      },
      goBack() {
        if (this.path_stack.length == 0) {
          console.log("cant go back any further")
          window.alert("cant go back any further")
          return
        }
        this.history_stack.push(this.path_stack[this.path_stack.length - 1])
        this.path_stack.pop()
        this.constructPath()
        loadFileInfo(this.current_path, this.handleFileList)
      },
      goForward() {
        if (this.history_stack.length == 0) {
          console.log("cant go forward any further")
          window.alert("cant go forward any further")
          return
        }
        this.path_stack.push(this.history_stack[this.history_stack.length - 1])
        this.history_stack.pop()
        this.constructPath()
        loadFileInfo(this.current_path, this.handleFileList)
      },
      handleFileList(res) {
        this.files = res
      },
      handleRequest(file) {
        if (file.isDir) {
          this.goToPath(file.name)
          loadFileInfo(this.current_path, this.handleFileList)
        } else {
          window.location.href = "/files" + this.current_path + file.name;
        }
      },
      selectFile(i) {
        if (this.selectedFileIndex == i) {
          this.handleRequest(this.files[i])
          return
        }
        this.selectedFileIndex = i
      },
      selectIcon(i) {
        if (this.files[i].isDir) {
          if (this.selectedFileIndex == i) return this.fileIcons.dirActive
          else return this.fileIcons.dir
        } else {
          if (this.selectedFileIndex == i) return this.fileIcons.fileActive
          else return this.fileIcons.file
        }
      },

    },
    data() {
      return {
        btnBackActive: false,
        btnForwardActive: false,
        btnParentActive: false,
        fileIcons: {
          dir: "/icons/folder.svg",
          dirActive: "/icons/folder-fill.svg",
          file: "/icons/file-earmark.svg",
          fileActive: "/icons/file-earmark-fill.svg"
        },
        files: [
          { name: "test.txt", isDir: false },
          { name: "12.mp4", isDir: false },
          { name: "porno.mkv", isDir: false },
          { name: "folder", isDir: true }
        ],
        selectedFileIndex: 0,

        current_path : "/",
        path_stack: [],
        history_stack: []
      }
    },
    mounted() {
      loadFileInfo(this.current_path, this.handleFileList)
    }
  }).mount('#app')
}

// load files from the dir
function loadFileInfo(dir, callback) {
  let request = new XMLHttpRequest()
  request.open("POST", '/list', true)
  request.send(dir)
  request.onreadystatechange = function () {
    if (request.readyState == 4 && request.status == 200) {
      let res = request.responseText
      let arr = JSON.parse(res)
      console.log(arr)
      callback(arr)
    }
  }
}
