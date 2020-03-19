import { Injectable } from '@angular/core';
import {Observable} from 'rxjs';
import * as io from 'socket.io-client';

@Injectable({
  providedIn: 'root'
})
export class WebsocketService {

  private url = 'http://localhost:9000';
  private socket;

  constructor() { 
    this.socket = io(this.url);
  }

  sendMessage(msg: string){
    this.socket.emit("new", msg);
  }

  getMessages = () => {
    return Observable.create((observer) => {
        this.socket.on("server", (message) => {
            observer.next(message);
        });
    });
}
}
