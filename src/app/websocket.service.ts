import { Injectable } from '@angular/core';
import { Socket } from 'ngx-socket-io';

@Injectable({
  providedIn: 'root'
})
export class WebsocketService {

  constructor(private socket: Socket) { }

  sendMessage(msg: string){
    this.socket.emit("message", msg);
  }
  // getMessage() {
  //     return this.socket
  //         .fromEvent("message")
  //         .map( data => data.msg );
  // }

}
