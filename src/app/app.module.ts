import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { HttpClientModule } from '@angular/common/http';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { SocketIoModule, SocketIoConfig } from 'ngx-socket-io';
import { StorageServiceModule} from 'angular-webstorage-service';
import { SessionDataService } from './session-data.service';
 
const config: SocketIoConfig = { url: 'http://localhost:500', options: {} };

@NgModule({
  declarations: [
    AppComponent,
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    HttpClientModule,
    StorageServiceModule,
    SocketIoModule.forRoot(config)
  ],
  providers: [SessionDataService],
  bootstrap: [AppComponent]
})
export class AppModule { }
