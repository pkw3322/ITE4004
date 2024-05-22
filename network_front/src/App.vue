<template>
  <div id="app">
    유저이름 <input v-model="userName" type="text" /> 내용
    <input v-model="message" type="text" />
    <div v-for="(item, idx) in recvList" :key="idx">
      <h3>유저이름: {{ item.userName }}</h3>
      <h3>내용: {{ item.message }}</h3>
    </div>
  </div>
  <img alt="Vue logo" src="./assets/logo.png" />
  <HelloWorld msg="Welcome to Your Vue.js App" />
</template>

<script>
import Stomp from 'webstomp-client'
import SockJs from 'sockjs-client'


export default {
  name: "App",
  data(){
    return {
      userName:"",
      message:"",
      recvList:[],
    };
  },
  created() {
    this.connect();
  },
  methods: {
    sendMessage(e){
      if(e.keyCode === 13 && this.userName !== "" && this.message !== ""){
        this.send()
        this.message=""
      }
    },
    send(){
      console.log("Send Message: " + this.message);
      if(this.stompClient && this.stompClient.connected){
        const msg = {
          userName: this.userName,
          message: this.message,
        };
        this.stompClient.send("/recieve", JSON.stringify(msg),{});
      }
    },
    connect() {
      const serverURL = "http://localhost:8080"
      let socket = new SockJs(serverURL);
      this.stompClient = stomp.over(socket);
      console.log("연결 시도: ", serverURL);
      this.stompClient.connect({},
        frame => {
          this.connected = true;
          console.log("연결!");
          this.stompClient.subscribe("/send",res => {
            this.recvList.push(JSON.parse(res.body));
          });
        },
        error => {
          this.connected = false;
          console.log("연결 실패!");
        }
      );
    },
  }
};
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}
</style>
